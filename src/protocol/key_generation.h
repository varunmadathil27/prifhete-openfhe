#ifndef PRIFHETE_PROTOCOL_KEY_GENERATION_H
#define PRIFHETE_PROTOCOL_KEY_GENERATION_H

#include "common/types.h"

namespace prifhete {

struct KeyGenerationResult {
    PrivateAccountPK public_key;
    PrivateAccountSK secret_key;
    Status status;
};

KeyGenerationResult KeyGeneration();

}  // namespace prifhete

#endif  // PRIFHETE_PROTOCOL_KEY_GENERATION_H
