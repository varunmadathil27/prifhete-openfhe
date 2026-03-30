#include "proof/proof_api.h"

namespace prifhete {

ProofBytes ProveRTransfer(const RTransferInstance& instance) {
    (void)instance;

    // TODO(prifhete): Implement the paper's R_TRANSFER proof generation.
    return ProofBytes{};
}

ProofVerificationResult VerifyRTransfer(const RTransferInstance& instance,
                                        const ProofBytes& proof) {
    (void)instance;
    (void)proof;

    // TODO(prifhete): Implement the paper's R_TRANSFER verification.
    return ProofVerificationResult{
        false,
        UnimplementedStatus("R_TRANSFER verification is not implemented")
    };
}

}  // namespace prifhete
