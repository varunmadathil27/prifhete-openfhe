#include "merkle/merkle_tree.h"

namespace prifhete {

MerkleRoot MerkleTree::Build(const std::vector<CommitmentBytes>& leaves) const {
    (void)leaves;

    // TODO(prifhete): Implement the Merkle tree construction required by the
    // paper's authenticated state.
    return MerkleRoot{};
}

bool MerkleTree::VerifyRoot(const std::vector<CommitmentBytes>& leaves,
                            const MerkleRoot& root) const {
    (void)leaves;
    (void)root;

    // TODO(prifhete): Implement Merkle root verification.
    return false;
}

}  // namespace prifhete
