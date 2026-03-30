#include "protocol/update_ciphertext.h"

namespace prifhete {

UpdateCiphertextResult UpdateCiphertext(const CiphertextBytes& prior_ciphertext,
                                        const ProofBytes& proof,
                                        EpochNumber epoch) {
    (void)proof;
    (void)epoch;

    // TODO(prifhete): Implement the paper's UpdateCiphertext algorithm. This
    // stub simply preserves the function boundary and returns the prior value.
    return UpdateCiphertextResult{
        prior_ciphertext,
        UnimplementedStatus("UpdateCiphertext is not implemented")
    };
}

}  // namespace prifhete
