#include "state/plaintext_model.h"

#include <algorithm>
#include <limits>
#include <sstream>

namespace prifhete {

namespace {

constexpr std::size_t kInvalidIndexValue = static_cast<std::size_t>(-1);

}  // namespace

PlaintextModel::PlaintextModel() {
    current_root_ = AllocateNextRoot();
}

bool PlaintextModel::HasAccount(const std::string& account_id) const {
    return accounts_.find(account_id) != accounts_.end();
}

Status PlaintextModel::ValidateAccountId(const std::string& account_id) const {
    if (account_id.empty()) {
        return Status{false, "account_id must not be empty"};
    }
    return OkStatus();
}

Status PlaintextModel::ValidateTransactionId(
    const std::string& transaction_id) const {
    if (transaction_id.empty()) {
        return Status{false, "transaction_id must not be empty"};
    }
    return OkStatus();
}

Status PlaintextModel::UpsertAccount(const PrivateAccountEntry& entry) {
    const Status account_id_status = ValidateAccountId(entry.account_id);
    if (!account_id_status.ok) {
        return account_id_status;
    }

    const auto existing_index = FindAccountIndex(entry.account_id);
    if (existing_index.has_value()) {
        AccountState& state = account_states_[existing_index->value];
        state.entry = entry;
        accounts_[entry.account_id] = entry;
        return OkStatus();
    }

    const std::size_t index = account_states_.size();
    account_states_.push_back(AccountState{entry, 0});
    account_indices_[entry.account_id] = index;
    accounts_[entry.account_id] = entry;
    return OkStatus();
}

const PrivateAccountEntry* PlaintextModel::FindAccount(
    const std::string& account_id) const {
    const auto iter = accounts_.find(account_id);
    if (iter == accounts_.end()) {
        return nullptr;
    }
    return &iter->second;
}

const PrivateAccountEntry* PlaintextModel::FindAccount(
    AccountIndex account_index) const {
    if (!IsValidAccountIndex(account_index)) {
        return nullptr;
    }
    return &account_states_[account_index.value].entry;
}

std::optional<PlaintextModel::AccountIndex> PlaintextModel::FindAccountIndex(
    const std::string& account_id) const {
    const auto iter = account_indices_.find(account_id);
    if (iter == account_indices_.end()) {
        return std::nullopt;
    }
    return AccountIndex{iter->second};
}

bool PlaintextModel::IsValidAccountIndex(AccountIndex account_index) const {
    return account_index.value != kInvalidIndexValue &&
           account_index.value < account_states_.size();
}

PlaintextModel::RegistrationResult PlaintextModel::Register(
    const RegistrationRequest& request) {
    const Status account_id_status = ValidateAccountId(request.account_id);
    if (!account_id_status.ok) {
        return RegistrationResult{
            AccountIndex{kInvalidIndexValue},
            PrivateAccountEntry{},
            current_root_,
            account_id_status
        };
    }

    if (HasAccount(request.account_id)) {
        return RegistrationResult{
            AccountIndex{kInvalidIndexValue},
            PrivateAccountEntry{},
            current_root_,
            Status{false, "account_id already exists"}
        };
    }

    PrivateAccountEntry entry;
    entry.account_id = request.account_id;
    entry.public_key = request.public_key;
    entry.epoch = current_epoch_;

    const std::size_t index = account_states_.size();
    account_states_.push_back(AccountState{entry, 0});
    account_indices_[entry.account_id] = index;

    current_root_ = AllocateNextRoot();
    RefreshAllAccountRoots();

    return RegistrationResult{
        AccountIndex{index},
        account_states_[index].entry,
        current_root_,
        OkStatus()
    };
}

bool PlaintextModel::IsRootAcceptableForCurrentEpoch(
    const RootReference& root) const {
    if (root.epoch > current_epoch_) {
        return false;
    }
    return current_epoch_ - root.epoch <= kRootValidityWindow;
}

Status PlaintextModel::ValidateReferencedRoot(const RootReference& root) const {
    const auto iter = valid_roots_.find(ToKey(root.root.bytes));
    if (iter == valid_roots_.end()) {
        return Status{false, "referenced_root is not known"};
    }

    if (iter->second.reference != root) {
        return Status{false, "referenced_root metadata mismatch"};
    }

    if (!IsRootAcceptableForCurrentEpoch(root)) {
        return Status{false, "referenced_root is stale"};
    }

    return OkStatus();
}

Status PlaintextModel::ValidateFreshPrfOutput(const PRFOutput& prf_output) const {
    const std::string key = ToKey(prf_output.bytes);
    if (prf_outputs_in_current_epoch_.find(key) != prf_outputs_in_current_epoch_.end()) {
        return Status{false, "duplicate PRFOutput in current epoch"};
    }
    return OkStatus();
}

Status PlaintextModel::RecordPrfOutput(const PRFOutput& prf_output) {
    const Status freshness_status = ValidateFreshPrfOutput(prf_output);
    if (!freshness_status.ok) {
        return freshness_status;
    }

    prf_outputs_in_current_epoch_.insert(ToKey(prf_output.bytes));
    accepted_prf_outputs_in_current_epoch_.push_back(prf_output);
    return OkStatus();
}

PlaintextModel::MintResult PlaintextModel::Mint(const PublicToPrivateMint& mint) {
    const Status transaction_id_status = ValidateTransactionId(mint.transaction_id);
    if (!transaction_id_status.ok) {
        return MintResult{current_root_, 0, transaction_id_status};
    }

    if (!IsValidAccountIndex(mint.recipient_index)) {
        return MintResult{current_root_, 0, Status{false, "invalid recipient account index"}};
    }

    const Status root_status = ValidateReferencedRoot(mint.referenced_root);
    if (!root_status.ok) {
        return MintResult{current_root_, 0, root_status};
    }

    const Status prf_status = ValidateFreshPrfOutput(mint.prf_output);
    if (!prf_status.ok) {
        return MintResult{current_root_, 0, prf_status};
    }

    AccountState& recipient = account_states_[mint.recipient_index.value];
    const Status record_status = RecordPrfOutput(mint.prf_output);
    if (!record_status.ok) {
        return MintResult{current_root_, recipient.balance, record_status};
    }

    recipient.balance += mint.value;
    current_root_ = AllocateNextRoot();
    RefreshAllAccountRoots();

    return MintResult{current_root_, recipient.balance, OkStatus()};
}

PlaintextModel::TransferResult PlaintextModel::Transfer(
    const PrivateTransfer& transfer) {
    const Status transaction_id_status = ValidateTransactionId(transfer.transaction_id);
    if (!transaction_id_status.ok) {
        return TransferResult{current_root_, 0, 0, transaction_id_status};
    }

    if (!IsValidAccountIndex(transfer.sender_index)) {
        return TransferResult{
            current_root_,
            0,
            0,
            Status{false, "invalid sender account index"}
        };
    }
    if (!IsValidAccountIndex(transfer.receiver_index)) {
        return TransferResult{
            current_root_,
            0,
            0,
            Status{false, "invalid receiver account index"}
        };
    }

    const Status root_status = ValidateReferencedRoot(transfer.referenced_root);
    if (!root_status.ok) {
        return TransferResult{current_root_, 0, 0, root_status};
    }

    const Status prf_status = ValidateFreshPrfOutput(transfer.prf_output);
    if (!prf_status.ok) {
        return TransferResult{current_root_, 0, 0, prf_status};
    }

    AccountState& sender = account_states_[transfer.sender_index.value];
    AccountState& receiver = account_states_[transfer.receiver_index.value];
    if (sender.balance < transfer.value) {
        return TransferResult{
            current_root_,
            sender.balance,
            receiver.balance,
            Status{false, "overspend rejected"}
        };
    }

    const Status record_status = RecordPrfOutput(transfer.prf_output);
    if (!record_status.ok) {
        return TransferResult{
            current_root_,
            sender.balance,
            receiver.balance,
            record_status
        };
    }

    sender.balance -= transfer.value;
    receiver.balance += transfer.value;
    current_root_ = AllocateNextRoot();
    RefreshAllAccountRoots();

    return TransferResult{
        current_root_,
        sender.balance,
        receiver.balance,
        OkStatus()
    };
}

Status PlaintextModel::ApplyEpochTransition(EpochNumber epoch) {
    current_epoch_ = epoch;
    prf_outputs_in_current_epoch_.clear();
    accepted_prf_outputs_in_current_epoch_.clear();
    current_epoch_root_keys_.clear();

    current_root_ = AllocateNextRoot();
    RefreshAllAccountRoots();

    return OkStatus();
}

Status PlaintextModel::AdvanceEpoch() {
    if (current_epoch_ == std::numeric_limits<EpochNumber>::max()) {
        return Status{false, "epoch overflow"};
    }
    return ApplyEpochTransition(current_epoch_ + 1);
}

Status PlaintextModel::AdvanceToEpoch(EpochNumber epoch) {
    if (epoch < current_epoch_) {
        return Status{false, "cannot move epoch backwards"};
    }
    while (current_epoch_ < epoch) {
        const Status step_status = AdvanceEpoch();
        if (!step_status.ok) {
            return step_status;
        }
    }
    return OkStatus();
}

bool PlaintextModel::TryGetBalance(AccountIndex account_index,
                                   std::uint64_t* balance) const {
    if (!IsValidAccountIndex(account_index) || balance == nullptr) {
        return false;
    }

    *balance = account_states_[account_index.value].balance;
    return true;
}

std::optional<std::uint64_t> PlaintextModel::InspectBalance(
    AccountIndex account_index) const {
    if (!IsValidAccountIndex(account_index)) {
        return std::nullopt;
    }
    return account_states_[account_index.value].balance;
}

std::uint64_t PlaintextModel::account_count() const {
    return static_cast<std::uint64_t>(account_states_.size());
}

MerkleRoot PlaintextModel::current_root() const {
    return current_root_.root;
}

PlaintextModel::RootReference PlaintextModel::current_root_reference() const {
    return current_root_;
}

bool PlaintextModel::IsValidRoot(const RootReference& root) const {
    return ValidateReferencedRoot(root).ok;
}

bool PlaintextModel::HasSeenPrfOutputInCurrentEpoch(
    const PRFOutput& prf_output) const {
    return prf_outputs_in_current_epoch_.find(ToKey(prf_output.bytes)) !=
           prf_outputs_in_current_epoch_.end();
}

std::vector<PRFOutput> PlaintextModel::accepted_prf_outputs() const {
    return accepted_prf_outputs_in_current_epoch_;
}

std::vector<PlaintextModel::RootReference> PlaintextModel::current_root_versions() const {
    std::vector<RootReference> roots;
    roots.reserve(current_epoch_root_keys_.size());
    for (const std::string& key : current_epoch_root_keys_) {
        const auto iter = valid_roots_.find(key);
        if (iter != valid_roots_.end()) {
            roots.push_back(iter->second.reference);
        }
    }
    return roots;
}

PlaintextModel::EpochStateView PlaintextModel::InspectEpochState() const {
    return EpochStateView{
        current_epoch_,
        current_root_.version,
        current_root_,
        accepted_prf_outputs_in_current_epoch_.size(),
        prf_outputs_in_current_epoch_.size()
    };
}

EpochNumber PlaintextModel::current_epoch() const {
    return current_epoch_;
}

void PlaintextModel::set_current_epoch(EpochNumber epoch) {
    if (epoch == current_epoch_) {
        return;
    }

    if (epoch < current_epoch_) {
        current_epoch_ = epoch;
        prf_outputs_in_current_epoch_.clear();
        accepted_prf_outputs_in_current_epoch_.clear();
        current_epoch_root_keys_.clear();
        current_root_ = AllocateNextRoot();
        RefreshAllAccountRoots();
        return;
    }

    const Status status = AdvanceToEpoch(epoch);
    if (!status.ok) {
        current_epoch_ = epoch;
    }
}

PlaintextModel::RootReference PlaintextModel::AllocateNextRoot() {
    ++root_version_.value;

    RootReference next_root;
    next_root.epoch = current_epoch_;
    next_root.version = root_version_;
    next_root.root = MakeRoot(current_epoch_, root_version_);

    const std::string key = ToKey(next_root.root.bytes);
    valid_roots_[key] = RootRecord{next_root};
    current_epoch_root_keys_.push_back(key);
    current_root_ = next_root;
    return next_root;
}

void PlaintextModel::RefreshAllAccountRoots() {
    for (AccountState& state : account_states_) {
        state.entry.epoch = current_epoch_;
        state.entry.inclusion_root = current_root_.root;
        accounts_[state.entry.account_id] = state.entry;
    }
}

MerkleRoot PlaintextModel::MakeRoot(EpochNumber epoch,
                                    RootVersion root_version) const {
    std::ostringstream out;
    out << "plaintext-root:e" << epoch << ":v" << root_version.value;

    const std::string encoded = out.str();
    return MerkleRoot{ByteString(encoded.begin(), encoded.end())};
}

std::string PlaintextModel::ToKey(const ByteString& bytes) {
    return std::string(bytes.begin(), bytes.end());
}

}  // namespace prifhete
