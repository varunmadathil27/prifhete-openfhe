#include "protocol/registration.h"

namespace prifhete {

RegistrationResult Registration(PlaintextModel& model,
                                const RegistrationRequest& request) {
    if (request.account_id.empty()) {
        return RegistrationResult{
            PrivateAccountEntry{},
            Status{false, "account_id must not be empty"}
        };
    }

    PrivateAccountEntry entry;
    entry.account_id = request.account_id;
    entry.public_key = request.public_key;
    entry.epoch = request.epoch;

    const Status status = model.UpsertAccount(entry);
    if (!status.ok) {
        return RegistrationResult{PrivateAccountEntry{}, status};
    }

    // TODO(prifhete): Replace this placeholder state insertion with the paper's
    // Registration algorithm, including commitments and authenticated state.
    return RegistrationResult{entry, UnimplementedStatus("Registration is not implemented")};
}

}  // namespace prifhete
