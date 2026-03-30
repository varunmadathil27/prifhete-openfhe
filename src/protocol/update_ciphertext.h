#ifndef PRIFHETE_PROTOCOL_UPDATE_CIPHERTEXT_H
#define PRIFHETE_PROTOCOL_UPDATE_CIPHERTEXT_H

#include "common/types.h"

namespace prifhete {

struct UpdateCiphertextResult {
    CiphertextBytes ciphertext;
    Status status;
};

UpdateCiphertextResult UpdateCiphertext(const CiphertextBytes& prior_ciphertext,
                                        const ProofBytes& proof,
                                        EpochNumber epoch);

}  // namespace prifhete

#endif  // PRIFHETE_PROTOCOL_UPDATE_CIPHERTEXT_H
