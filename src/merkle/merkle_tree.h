#ifndef PRIFHETE_MERKLE_MERKLE_TREE_H
#define PRIFHETE_MERKLE_MERKLE_TREE_H

#include <vector>

#include "common/types.h"

namespace prifhete {

class MerkleTree {
public:
    MerkleRoot Build(const std::vector<CommitmentBytes>& leaves) const;
    bool VerifyRoot(const std::vector<CommitmentBytes>& leaves,
                    const MerkleRoot& root) const;
};

}  // namespace prifhete

#endif  // PRIFHETE_MERKLE_MERKLE_TREE_H
