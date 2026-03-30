#include "fhe/circuits.h"

namespace prifhete {

BitValue EqualBitVectors(const BitBackend& backend,
                         const BitVector& lhs,
                         const BitVector& rhs) {
    if (lhs.size() != rhs.size()) {
        return backend.Constant(false);
    }

    BitVector equal_bits;
    equal_bits.reserve(lhs.size());
    for (std::size_t i = 0; i < lhs.size(); ++i) {
        equal_bits.push_back(backend.Not(backend.Xor(lhs[i], rhs[i])));
    }

    return ReduceAnd(backend, equal_bits);
}

BitValue ReduceAnd(const BitBackend& backend, const BitVector& bits) {
    BitValue accumulated = backend.Constant(true);
    for (const BitValue& bit : bits) {
        accumulated = backend.And(accumulated, bit);
    }
    return accumulated;
}

FullAdderResult FullAdder(const BitBackend& backend,
                          const BitValue& lhs,
                          const BitValue& rhs,
                          const BitValue& carry_in) {
    const BitValue lhs_xor_rhs = backend.Xor(lhs, rhs);
    const BitValue sum = backend.Xor(lhs_xor_rhs, carry_in);

    const BitValue carry_from_inputs = backend.And(lhs, rhs);
    const BitValue carry_from_carry_in = backend.And(lhs_xor_rhs, carry_in);
    const BitValue carry_out =
        backend.Or(carry_from_inputs, carry_from_carry_in);

    return FullAdderResult{sum, carry_out};
}

FullSubtracterResult FullSubtracter(const BitBackend& backend,
                                    const BitValue& lhs,
                                    const BitValue& rhs,
                                    const BitValue& borrow_in) {
    const BitValue lhs_xor_rhs = backend.Xor(lhs, rhs);
    const BitValue difference = backend.Xor(lhs_xor_rhs, borrow_in);

    const BitValue not_lhs = backend.Not(lhs);
    const BitValue rhs_or_borrow_in = backend.Or(rhs, borrow_in);
    const BitValue borrow_from_lhs = backend.And(not_lhs, rhs_or_borrow_in);
    const BitValue borrow_from_chain = backend.And(rhs, borrow_in);
    const BitValue borrow_out =
        backend.Or(borrow_from_lhs, borrow_from_chain);

    return FullSubtracterResult{difference, borrow_out};
}

}  // namespace prifhete
