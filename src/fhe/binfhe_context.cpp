#include "fhe/binfhe_context.h"

#include <cassert>

namespace prifhete {

BinFHEContext::BinFHEContext() = default;

bool BinFHEContext::enabled() const {
    return enabled_;
}

Status BinFHEContext::Initialize() {
#if PRIFHETE_ENABLE_OPENFHE
    enabled_ = true;
    return UnimplementedStatus("OpenFHE BinFHE adapter is not implemented");
#else
    enabled_ = false;
    return UnimplementedStatus("OpenFHE support is disabled");
#endif
}

BitBackendKind BinFHEContext::kind() const {
    return BitBackendKind::kBinFHE;
}

BitValue BinFHEContext::Constant(bool value) const {
    (void)value;
    assert(enabled_);
    return BitValue::BinFHEPlaceholder();
}

BitValue BinFHEContext::Xor(const BitValue& lhs, const BitValue& rhs) const {
    (void)lhs;
    (void)rhs;
    assert(enabled_);
    return BitValue::BinFHEPlaceholder();
}

BitValue BinFHEContext::And(const BitValue& lhs, const BitValue& rhs) const {
    (void)lhs;
    (void)rhs;
    assert(enabled_);
    return BitValue::BinFHEPlaceholder();
}

BitValue BinFHEContext::Or(const BitValue& lhs, const BitValue& rhs) const {
    (void)lhs;
    (void)rhs;
    assert(enabled_);
    return BitValue::BinFHEPlaceholder();
}

BitValue BinFHEContext::Not(const BitValue& input) const {
    (void)input;
    assert(enabled_);
    return BitValue::BinFHEPlaceholder();
}

}  // namespace prifhete
