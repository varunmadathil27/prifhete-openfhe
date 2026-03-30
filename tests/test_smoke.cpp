#include <cassert>

#include "common/types.h"
#include "fhe/binfhe_context.h"
#include "protocol/key_generation.h"
#include "protocol/registration.h"
#include "state/plaintext_model.h"

int main() {
    using namespace prifhete;

    static_assert(sizeof(EpochNumber) >= sizeof(std::uint64_t),
                  "EpochNumber should be an integer epoch type");

    PlaintextModel model;
    model.set_current_epoch(7);
    assert(model.current_epoch() == 7);

    const KeyGenerationResult keygen = KeyGeneration();
    assert(!keygen.status.ok);

    RegistrationRequest request;
    request.account_id = "alice";
    request.public_key = PrivateAccountPK{};
    request.epoch = model.current_epoch();

    const RegistrationResult registration = Registration(model, request);
    assert(!registration.status.ok);
    assert(model.HasAccount("alice"));

    const PrivateAccountEntry* alice = model.FindAccount("alice");
    assert(alice != nullptr);
    assert(alice->account_id == "alice");
    assert(alice->epoch == 7);

    BinFHEContext context;
    const Status init_status = context.Initialize();
    assert(!init_status.ok);
    assert(!context.enabled());

    return 0;
}
