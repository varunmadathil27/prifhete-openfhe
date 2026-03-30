#include "protocol/key_generation.h"

namespace prifhete {

KeyGenerationResult KeyGeneration() {
    // TODO(prifhete): Implement the paper's KeyGeneration algorithm with the
    // designated cryptographic backends. This stub only preserves the API.
    return KeyGenerationResult{
        PrivateAccountPK{},
        PrivateAccountSK{},
        UnimplementedStatus("KeyGeneration is not implemented")
    };
}

}  // namespace prifhete
