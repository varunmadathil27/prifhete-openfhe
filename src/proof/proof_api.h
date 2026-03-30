#ifndef PRIFHETE_PROOF_PROOF_API_H
#define PRIFHETE_PROOF_PROOF_API_H

#include "common/types.h"

namespace prifhete {

struct RTransferInstance {
    TransferTx transaction;
    MerkleRoot state_root;
};

struct ProofVerificationResult {
    bool verified = false;
    Status status;
};

ProofBytes ProveRTransfer(const RTransferInstance& instance);
ProofVerificationResult VerifyRTransfer(const RTransferInstance& instance,
                                        const ProofBytes& proof);

}  // namespace prifhete

#endif  // PRIFHETE_PROOF_PROOF_API_H
