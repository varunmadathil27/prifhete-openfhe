#ifndef PRIFHETE_PROTOCOL_TRANSFER_H
#define PRIFHETE_PROTOCOL_TRANSFER_H

#include "common/types.h"
#include "state/plaintext_model.h"

namespace prifhete {

struct TransferResult {
    TransferTx transaction;
    Status status;
};

TransferResult Transfer(PlaintextModel& model, const TransferTx& transaction);

}  // namespace prifhete

#endif  // PRIFHETE_PROTOCOL_TRANSFER_H
