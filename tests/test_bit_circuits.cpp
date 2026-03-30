#include <cassert>
#include <cstddef>
#include <vector>

#include "fhe/binfhe_context.h"
#include "fhe/circuits.h"

namespace {

using prifhete::BitValue;
using prifhete::BitVector;
using prifhete::FakeBitContext;
using prifhete::FullAdder;
using prifhete::FullSubtracter;
using prifhete::ReduceAnd;
using prifhete::EqualBitVectors;

bool ReadFakeBit(const BitValue& bit) {
    assert(bit.IsFake());
    return bit.fake_value();
}

BitValue MakeFakeBit(bool value) {
    return BitValue::Fake(value);
}

BitVector BitsFromMask(std::size_t width, std::size_t mask) {
    BitVector bits;
    bits.reserve(width);
    for (std::size_t i = 0; i < width; ++i) {
        bits.push_back(MakeFakeBit(((mask >> i) & 1U) != 0U));
    }
    return bits;
}

void TestPrimitiveTruthTables() {
    const FakeBitContext backend;
    for (int lhs = 0; lhs <= 1; ++lhs) {
        for (int rhs = 0; rhs <= 1; ++rhs) {
            const BitValue left = MakeFakeBit(lhs != 0);
            const BitValue right = MakeFakeBit(rhs != 0);

            assert(ReadFakeBit(backend.Xor(left, right)) == ((lhs ^ rhs) != 0));
            assert(ReadFakeBit(backend.And(left, right)) == ((lhs & rhs) != 0));
            assert(ReadFakeBit(backend.Or(left, right)) == ((lhs | rhs) != 0));
        }
    }

    assert(ReadFakeBit(backend.Not(MakeFakeBit(false))) == true);
    assert(ReadFakeBit(backend.Not(MakeFakeBit(true))) == false);
}

void TestReduceAndTruthTables() {
    const FakeBitContext backend;

    assert(ReadFakeBit(ReduceAnd(backend, {})) == true);

    for (std::size_t width = 1; width <= 4; ++width) {
        const std::size_t combinations = static_cast<std::size_t>(1) << width;
        for (std::size_t mask = 0; mask < combinations; ++mask) {
            const bool expected = mask == (combinations - 1);
            assert(ReadFakeBit(ReduceAnd(backend, BitsFromMask(width, mask))) ==
                   expected);
        }
    }
}

void TestEqualityTruthTables() {
    const FakeBitContext backend;

    assert(ReadFakeBit(EqualBitVectors(backend, {}, {})) == true);
    assert(ReadFakeBit(
               EqualBitVectors(backend, BitsFromMask(2, 1), BitsFromMask(3, 1))) ==
           false);

    for (std::size_t width = 1; width <= 4; ++width) {
        const std::size_t combinations = static_cast<std::size_t>(1) << width;
        for (std::size_t lhs = 0; lhs < combinations; ++lhs) {
            for (std::size_t rhs = 0; rhs < combinations; ++rhs) {
                assert(ReadFakeBit(EqualBitVectors(backend,
                                                   BitsFromMask(width, lhs),
                                                   BitsFromMask(width, rhs))) ==
                       (lhs == rhs));
            }
        }
    }
}

void TestFullAdderTruthTable() {
    const FakeBitContext backend;
    for (int lhs = 0; lhs <= 1; ++lhs) {
        for (int rhs = 0; rhs <= 1; ++rhs) {
            for (int carry_in = 0; carry_in <= 1; ++carry_in) {
                const auto result = FullAdder(
                    backend, MakeFakeBit(lhs != 0), MakeFakeBit(rhs != 0),
                    MakeFakeBit(carry_in != 0));
                const int total = lhs + rhs + carry_in;
                assert(ReadFakeBit(result.sum) == ((total & 1) != 0));
                assert(ReadFakeBit(result.carry_out) == (total >= 2));
            }
        }
    }
}

void TestFullSubtracterTruthTable() {
    const FakeBitContext backend;
    for (int lhs = 0; lhs <= 1; ++lhs) {
        for (int rhs = 0; rhs <= 1; ++rhs) {
            for (int borrow_in = 0; borrow_in <= 1; ++borrow_in) {
                const auto result = FullSubtracter(
                    backend, MakeFakeBit(lhs != 0), MakeFakeBit(rhs != 0),
                    MakeFakeBit(borrow_in != 0));

                const int expected = lhs - rhs - borrow_in;
                const bool expected_borrow = expected < 0;
                const int normalized = expected_borrow ? expected + 2 : expected;

                assert(ReadFakeBit(result.difference) == ((normalized & 1) != 0));
                assert(ReadFakeBit(result.borrow_out) == expected_borrow);
            }
        }
    }
}

void TestBinFHEPlaceholderSurface() {
    prifhete::BinFHEContext context;
    const prifhete::Status status = context.Initialize();
    assert(!status.ok);
    assert(!context.enabled());
    assert(context.kind() == prifhete::BitBackendKind::kBinFHE);
}

}  // namespace

int main() {
    TestPrimitiveTruthTables();
    TestReduceAndTruthTables();
    TestEqualityTruthTables();
    TestFullAdderTruthTable();
    TestFullSubtracterTruthTable();
    TestBinFHEPlaceholderSurface();
    return 0;
}
