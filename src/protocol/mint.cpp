#include "protocol/mint.h"

namespace prifhete {

MintResult Mint(PlaintextModel& model, const MintTx& transaction) {
    (void)model;

    // TODO(prifhete): Implement the paper's Mint algorithm. This stub only
    // validates a minimal shape and does not perform any cryptographic action.
    if (transaction.recipient_account_id.empty()) {
        return MintResult{MintTx{}, Status{false, "recipient_account_id must not be empty"}};
    }

    return MintResult{transaction, UnimplementedStatus("Mint is not implemented")};
}

}  // namespace prifhete
