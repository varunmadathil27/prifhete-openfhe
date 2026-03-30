#include "protocol/process_transaction.h"

#include "protocol/mint.h"
#include "protocol/transfer.h"

namespace prifhete {

ProcessTransactionResult ProcessMintTransaction(PlaintextModel& model,
                                                const MintTx& transaction) {
    const MintResult result = Mint(model, transaction);

    // TODO(prifhete): Implement the paper's ProcessTransaction logic for mint
    // transactions, including proof verification and state commitments.
    return ProcessTransactionResult{result.status};
}

ProcessTransactionResult ProcessTransferTransaction(PlaintextModel& model,
                                                    const TransferTx& transaction) {
    const TransferResult result = Transfer(model, transaction);

    // TODO(prifhete): Implement the paper's ProcessTransaction logic for
    // transfers, including R_TRANSFER verification and state updates.
    return ProcessTransactionResult{result.status};
}

}  // namespace prifhete
