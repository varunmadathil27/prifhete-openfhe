#ifndef PRIFHETE_STATE_PLAINTEXT_MODEL_H
#define PRIFHETE_STATE_PLAINTEXT_MODEL_H

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>

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

    struct RegistrationRequest {
        std::string account_id;
        PrivateAccountPK public_key;
    };

    struct RegistrationResult {
        AccountIndex account_index;
        PrivateAccountEntry entry;
        MerkleRoot root;
        Status status;
    };

    struct PublicToPrivateMint {
        std::string transaction_id;
        AccountIndex recipient_index;
        std::uint64_t value = 0;
        PRFOutput prf_output;
        MerkleRoot referenced_root;
    };

    struct MintResult {
        MerkleRoot root;
        std::uint64_t recipient_balance = 0;
        Status status;
    };

    struct PrivateTransfer {
        std::string transaction_id;
        AccountIndex sender_index;
        AccountIndex receiver_index;
        std::uint64_t value = 0;
        PRFOutput prf_output;
        MerkleRoot referenced_root;
    };

    struct TransferResult {
        MerkleRoot root;
        std::uint64_t sender_balance = 0;
        std::uint64_t receiver_balance = 0;
        Status status;
    };

    struct EpochStateView {
        EpochNumber current_epoch = 0;
        std::uint64_t root_version = 0;
        MerkleRoot current_root;
        std::size_t valid_root_count = 0;
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
    std::uint64_t account_count() const;
    MerkleRoot current_root() const;
    bool IsValidRoot(const MerkleRoot& root) const;
    bool HasSeenPrfOutputInCurrentEpoch(const PRFOutput& prf_output) const;
    EpochStateView InspectEpochState() const;

    EpochNumber current_epoch() const;
    void set_current_epoch(EpochNumber epoch);

private:
    struct AccountState {
        PrivateAccountEntry entry;
        std::uint64_t balance = 0;
    };

    static constexpr EpochNumber kRootValidityWindow = 1;

    Status ValidateAccountId(const std::string& account_id) const;
    bool IsRootAcceptableForCurrentEpoch(EpochNumber root_epoch) const;
    Status ValidateReferencedRoot(const MerkleRoot& root) const;
    Status ValidateFreshPrfOutput(const PRFOutput& prf_output) const;
    Status RecordPrfOutput(const PRFOutput& prf_output);
    Status ApplyEpochTransition(EpochNumber epoch);
    MerkleRoot MakeRoot(EpochNumber epoch, std::uint64_t root_version) const;
    static std::string ToKey(const ByteString& bytes);

    std::unordered_map<std::string, PrivateAccountEntry> accounts_;
    std::vector<AccountState> account_states_;
    std::unordered_map<std::string, std::size_t> account_indices_;
    std::unordered_map<std::string, EpochNumber> valid_roots_;
    std::unordered_set<std::string> prf_outputs_in_current_epoch_;
    EpochNumber current_epoch_ = 0;
    std::uint64_t root_version_ = 0;
    MerkleRoot current_root_;
};

}  // namespace prifhete

#endif  // PRIFHETE_STATE_PLAINTEXT_MODEL_H
