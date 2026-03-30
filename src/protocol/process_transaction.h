#ifndef PRIFHETE_PROTOCOL_PROCESS_TRANSACTION_H
#define PRIFHETE_PROTOCOL_PROCESS_TRANSACTION_H

#include "common/types.h"
#include "state/plaintext_model.h"

namespace prifhete {

struct ProcessTransactionResult {
    Status status;
};

ProcessTransactionResult ProcessMintTransaction(PlaintextModel& model,
                                                const MintTx& transaction);
ProcessTransactionResult ProcessTransferTransaction(PlaintextModel& model,
                                                    const TransferTx& transaction);

}  // namespace prifhete

#endif  // PRIFHETE_PROTOCOL_PROCESS_TRANSACTION_H
