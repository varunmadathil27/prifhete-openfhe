#include "protocol/transfer.h"

namespace prifhete {

TransferResult Transfer(PlaintextModel& model, const TransferTx& transaction) {
    (void)model;

    // TODO(prifhete): Implement the paper's Transfer algorithm and preserve the
    // separate sender/receiver ciphertext evolution described in the paper.
    if (transaction.sender_account_id.empty() ||
        transaction.receiver_account_id.empty()) {
        return TransferResult{
            TransferTx{},
            Status{false, "sender_account_id and receiver_account_id must not be empty"}
        };
    }

    return TransferResult{transaction, UnimplementedStatus("Transfer is not implemented")};
}

}  // namespace prifhete
