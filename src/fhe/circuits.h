#ifndef PRIFHETE_FHE_CIRCUITS_H
#define PRIFHETE_FHE_CIRCUITS_H

#include "fhe/bit_value.h"

namespace prifhete {

struct FullAdderResult {
    BitValue sum;
    BitValue carry_out;
};

struct FullSubtracterResult {
    BitValue difference;
    BitValue borrow_out;
};

BitValue EqualBitVectors(const BitBackend& backend,
                         const BitVector& lhs,
                         const BitVector& rhs);

BitValue ReduceAnd(const BitBackend& backend, const BitVector& bits);

FullAdderResult FullAdder(const BitBackend& backend,
                          const BitValue& lhs,
                          const BitValue& rhs,
                          const BitValue& carry_in);

FullSubtracterResult FullSubtracter(const BitBackend& backend,
                                    const BitValue& lhs,
                                    const BitValue& rhs,
                                    const BitValue& borrow_in);

}  // namespace prifhete

#endif  // PRIFHETE_FHE_CIRCUITS_H
