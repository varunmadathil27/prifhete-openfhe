#ifndef PRIFHETE_COMMON_TYPES_H
#define PRIFHETE_COMMON_TYPES_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace prifhete {

using ByteString = std::vector<std::uint8_t>;
using EpochNumber = std::uint64_t;

struct CiphertextBytes {
    ByteString bytes;
};

struct ProofBytes {
    ByteString bytes;
};

struct CommitmentBytes {
    ByteString bytes;
};

struct MerkleRoot {
    ByteString bytes;
};

struct PRFOutput {
    ByteString bytes;
};

struct PrivateAccountPK {
    ByteString bytes;
};

struct PrivateAccountSK {
    ByteString bytes;
};

struct PrivateAccountEntry {
    std::string account_id;
    PrivateAccountPK public_key;
    CiphertextBytes ciphertext;
    CommitmentBytes commitment;
    MerkleRoot inclusion_root;
    EpochNumber epoch = 0;
};

struct MintTx {
    std::string transaction_id;
    std::string recipient_account_id;
    std::uint64_t value = 0;
    CiphertextBytes recipient_ciphertext;
    ProofBytes proof;
    EpochNumber epoch = 0;
};

struct TransferTx {
    std::string transaction_id;
    std::string sender_account_id;
    std::string receiver_account_id;
    CiphertextBytes sender_ciphertext;
    CiphertextBytes receiver_ciphertext;
    ProofBytes proof;
    EpochNumber epoch = 0;
};

struct Status {
    bool ok = false;
    std::string message;
};

inline Status OkStatus() {
    return Status{true, "ok"};
}

inline Status UnimplementedStatus(const std::string& message) {
    return Status{false, message};
}

}  // namespace prifhete

#endif  // PRIFHETE_COMMON_TYPES_H
