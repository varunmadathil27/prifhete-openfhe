#ifndef PRIFHETE_FHE_BIT_VALUE_H
#define PRIFHETE_FHE_BIT_VALUE_H

#include <cassert>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

namespace prifhete {

enum class BitBackendKind : std::uint8_t {
    kFake = 0,
    kBinFHE = 1,
};

class BitValue {
public:
    BitValue() = default;

    static BitValue Fake(bool value) {
        return BitValue(BitBackendKind::kFake, value, nullptr);
    }

    static BitValue BinFHEPlaceholder() {
        return BitValue(BitBackendKind::kBinFHE, false, nullptr);
    }

    BitBackendKind backend() const {
        return backend_;
    }

    bool IsFake() const {
        return backend_ == BitBackendKind::kFake;
    }

    bool fake_value() const {
        assert(IsFake());
        return fake_value_;
    }

private:
    BitValue(BitBackendKind backend, bool fake_value,
             std::shared_ptr<const void> opaque_payload)
        : backend_(backend),
          fake_value_(fake_value),
          opaque_payload_(std::move(opaque_payload)) {}

    BitBackendKind backend_ = BitBackendKind::kFake;
    bool fake_value_ = false;
    std::shared_ptr<const void> opaque_payload_;
};

using BitVector = std::vector<BitValue>;

class BitBackend {
public:
    virtual ~BitBackend() = default;

    virtual BitBackendKind kind() const = 0;
    virtual BitValue Constant(bool value) const = 0;
    virtual BitValue Xor(const BitValue& lhs, const BitValue& rhs) const = 0;
    virtual BitValue And(const BitValue& lhs, const BitValue& rhs) const = 0;
    virtual BitValue Or(const BitValue& lhs, const BitValue& rhs) const = 0;
    virtual BitValue Not(const BitValue& input) const = 0;
};

class FakeBitContext final : public BitBackend {
public:
    BitBackendKind kind() const override {
        return BitBackendKind::kFake;
    }

    BitValue Constant(bool value) const override {
        return BitValue::Fake(value);
    }

    BitValue Xor(const BitValue& lhs, const BitValue& rhs) const override {
        assert(lhs.IsFake());
        assert(rhs.IsFake());
        return BitValue::Fake(lhs.fake_value() != rhs.fake_value());
    }

    BitValue And(const BitValue& lhs, const BitValue& rhs) const override {
        assert(lhs.IsFake());
        assert(rhs.IsFake());
        return BitValue::Fake(lhs.fake_value() && rhs.fake_value());
    }

    BitValue Or(const BitValue& lhs, const BitValue& rhs) const override {
        assert(lhs.IsFake());
        assert(rhs.IsFake());
        return BitValue::Fake(lhs.fake_value() || rhs.fake_value());
    }

    BitValue Not(const BitValue& input) const override {
        assert(input.IsFake());
        return BitValue::Fake(!input.fake_value());
    }
};

}  // namespace prifhete

#endif  // PRIFHETE_FHE_BIT_VALUE_H
