#ifndef PRIFHETE_PROTOCOL_REGISTRATION_H
#define PRIFHETE_PROTOCOL_REGISTRATION_H

#include <string>

#include "common/types.h"
#include "state/plaintext_model.h"

namespace prifhete {

struct RegistrationRequest {
    std::string account_id;
    PrivateAccountPK public_key;
    EpochNumber epoch = 0;
};

struct RegistrationResult {
    PrivateAccountEntry entry;
    Status status;
};

RegistrationResult Registration(PlaintextModel& model,
                                const RegistrationRequest& request);

}  // namespace prifhete

#endif  // PRIFHETE_PROTOCOL_REGISTRATION_H
