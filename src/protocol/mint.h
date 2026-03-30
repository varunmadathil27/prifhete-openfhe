#ifndef PRIFHETE_PROTOCOL_MINT_H
#define PRIFHETE_PROTOCOL_MINT_H

#include "common/types.h"
#include "state/plaintext_model.h"

namespace prifhete {

struct MintResult {
    MintTx transaction;
    Status status;
};

MintResult Mint(PlaintextModel& model, const MintTx& transaction);

}  // namespace prifhete

#endif  // PRIFHETE_PROTOCOL_MINT_H
