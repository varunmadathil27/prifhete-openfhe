#ifndef PRIFHETE_STATE_PLAINTEXT_MODEL_H
#define PRIFHETE_STATE_PLAINTEXT_MODEL_H

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common/types.h"

namespace prifhete {

class PlaintextModel {
public:
    struct AccountIndex {
        std::size_t value = static_cast<std::size_t>(-1);

        bool operator==(const AccountIndex& other) const {
            return value == other.value;
        }

        bool operator!=(const AccountIndex& other) const {
            return !(*this == other);
        }
    };

    struct RootVersion {
        std::uint64_t value = 0;

        bool operator==(const RootVersion& other) const {
            return value == other.value;
        }

        bool operator!=(const RootVersion& other) const {
            return !(*this == other);
        }
    };

    struct RootReference {
        MerkleRoot root;
        EpochNumber epoch = 0;
        RootVersion version;

        bool operator==(const RootReference& other) const {
            return root.bytes == other.root.bytes &&
                   epoch == other.epoch &&
                   version == other.version;
        }

        bool operator!=(const RootReference& other) const {
            return !(*this == other);
        }
    };

    struct RegistrationRequest {
        std::string account_id;
        PrivateAccountPK public_key;
    };

    struct RegistrationResult {
        AccountIndex account_index;
        PrivateAccountEntry entry;
        RootReference root;
        Status status;
    };

    struct PublicToPrivateMint {
        std::string transaction_id;
        AccountIndex recipient_index;
        std::uint64_t value = 0;
        PRFOutput prf_output;
        RootReference referenced_root;
    };

    struct MintResult {
        RootReference root;
        std::uint64_t recipient_balance = 0;
        Status status;
    };

    struct PrivateTransfer {
        std::string transaction_id;
        AccountIndex sender_index;
        AccountIndex receiver_index;
        std::uint64_t value = 0;
        PRFOutput prf_output;
        RootReference referenced_root;
    };

    struct TransferResult {
        RootReference root;
        std::uint64_t sender_balance = 0;
        std::uint64_t receiver_balance = 0;
        Status status;
    };

    struct EpochStateView {
        EpochNumber current_epoch = 0;
        RootVersion current_root_version;
        RootReference current_root;
        std::size_t accepted_prf_output_count = 0;
        std::size_t used_prf_output_count = 0;
    };

    PlaintextModel();

    bool HasAccount(const std::string& account_id) const;
    Status UpsertAccount(const PrivateAccountEntry& entry);
    const PrivateAccountEntry* FindAccount(const std::string& account_id) const;
    const PrivateAccountEntry* FindAccount(AccountIndex account_index) const;

    std::optional<AccountIndex> FindAccountIndex(const std::string& account_id) const;
    bool IsValidAccountIndex(AccountIndex account_index) const;

    RegistrationResult Register(const RegistrationRequest& request);
    MintResult Mint(const PublicToPrivateMint& mint);
    TransferResult Transfer(const PrivateTransfer& transfer);

    Status AdvanceEpoch();
    Status AdvanceToEpoch(EpochNumber epoch);

    bool TryGetBalance(AccountIndex account_index, std::uint64_t* balance) const;
    std::optional<std::uint64_t> InspectBalance(AccountIndex account_index) const;
    std::uint64_t account_count() const;
    MerkleRoot current_root() const;
    RootReference current_root_reference() const;
    bool IsValidRoot(const RootReference& root) const;
    bool HasSeenPrfOutputInCurrentEpoch(const PRFOutput& prf_output) const;
    std::vector<PRFOutput> accepted_prf_outputs() const;
    std::vector<RootReference> current_root_versions() const;
    EpochStateView InspectEpochState() const;

    EpochNumber current_epoch() const;
    void set_current_epoch(EpochNumber epoch);

private:
    struct AccountState {
        PrivateAccountEntry entry;
        std::uint64_t balance = 0;
    };

    struct RootRecord {
        RootReference reference;
    };

    static constexpr EpochNumber kRootValidityWindow = 1;

    Status ValidateAccountId(const std::string& account_id) const;
    Status ValidateTransactionId(const std::string& transaction_id) const;
    bool IsRootAcceptableForCurrentEpoch(const RootReference& root) const;
    Status ValidateReferencedRoot(const RootReference& root) const;
    Status ValidateFreshPrfOutput(const PRFOutput& prf_output) const;
    Status RecordPrfOutput(const PRFOutput& prf_output);
    Status ApplyEpochTransition(EpochNumber epoch);
    RootReference AllocateNextRoot();
    void RefreshAllAccountRoots();
    MerkleRoot MakeRoot(EpochNumber epoch, RootVersion root_version) const;
    static std::string ToKey(const ByteString& bytes);

    std::unordered_map<std::string, PrivateAccountEntry> accounts_;
    std::vector<AccountState> account_states_;
    std::unordered_map<std::string, std::size_t> account_indices_;
    std::unordered_map<std::string, RootRecord> valid_roots_;
    std::vector<std::string> current_epoch_root_keys_;
    std::unordered_set<std::string> prf_outputs_in_current_epoch_;
    std::vector<PRFOutput> accepted_prf_outputs_in_current_epoch_;
    EpochNumber current_epoch_ = 0;
    RootVersion root_version_;
    RootReference current_root_;
};

}  // namespace prifhete

#endif  // PRIFHETE_STATE_PLAINTEXT_MODEL_H
