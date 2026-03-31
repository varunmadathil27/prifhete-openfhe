#include "fhe/binfhe_context.h"

#include <cassert>
#include <memory>
#include <string>

namespace prifhete {

struct BinFHEContext::Impl {
#if PRIFHETE_ENABLE_OPENFHE
    BitValue Constant(bool value) const;
    BitValue Xor(const BitValue& lhs, const BitValue& rhs) const;
    BitValue And(const BitValue& lhs, const BitValue& rhs) const;
    BitValue Or(const BitValue& lhs, const BitValue& rhs) const;
    BitValue Not(const BitValue& input) const;
#endif
};

#if PRIFHETE_ENABLE_OPENFHE
namespace {

struct PlaceholderBinFHEBit {
    std::string label;
};

std::shared_ptr<const void> MakePlaceholderHandle(const std::string& label) {
    return std::static_pointer_cast<const void>(
        std::make_shared<PlaceholderBinFHEBit>(PlaceholderBinFHEBit{label}));
}

void AssertBinFHEBit(const BitValue& bit) {
    assert(bit.IsBinFHE());
    assert(bit.has_opaque_payload());
}

}  // namespace

BitValue BinFHEContext::Impl::Constant(bool value) const {
    return BitValue::BinFHEPlaceholder(
        MakePlaceholderHandle(value ? "const-1" : "const-0"));
}

BitValue BinFHEContext::Impl::Xor(const BitValue& lhs,
                                  const BitValue& rhs) const {
    AssertBinFHEBit(lhs);
    AssertBinFHEBit(rhs);
    return BitValue::BinFHEPlaceholder(MakePlaceholderHandle("xor"));
}

BitValue BinFHEContext::Impl::And(const BitValue& lhs,
                                  const BitValue& rhs) const {
    AssertBinFHEBit(lhs);
    AssertBinFHEBit(rhs);
    return BitValue::BinFHEPlaceholder(MakePlaceholderHandle("and"));
}

BitValue BinFHEContext::Impl::Or(const BitValue& lhs,
                                 const BitValue& rhs) const {
    AssertBinFHEBit(lhs);
    AssertBinFHEBit(rhs);
    return BitValue::BinFHEPlaceholder(MakePlaceholderHandle("or"));
}

BitValue BinFHEContext::Impl::Not(const BitValue& input) const {
    AssertBinFHEBit(input);
    return BitValue::BinFHEPlaceholder(MakePlaceholderHandle("not"));
}
#endif

BinFHEContext::BinFHEContext() = default;
BinFHEContext::~BinFHEContext() = default;

bool BinFHEContext::enabled() const {
    return enabled_;
}

Status BinFHEContext::Initialize() {
#if PRIFHETE_ENABLE_OPENFHE
    impl_ = std::make_unique<Impl>();
    enabled_ = true;
    return OkStatus();
#else
    impl_.reset();
    enabled_ = false;
    return UnimplementedStatus("OpenFHE support is disabled");
#endif
}

BitBackendKind BinFHEContext::kind() const {
    return BitBackendKind::kBinFHE;
}

BitValue BinFHEContext::Constant(bool value) const {
    assert(enabled_);
#if PRIFHETE_ENABLE_OPENFHE
    assert(impl_ != nullptr);
    return impl_->Constant(value);
#else
    (void)value;
    return BitValue::BinFHEPlaceholder();
#endif
}

BitValue BinFHEContext::Xor(const BitValue& lhs, const BitValue& rhs) const {
    assert(enabled_);
#if PRIFHETE_ENABLE_OPENFHE
    assert(impl_ != nullptr);
    return impl_->Xor(lhs, rhs);
#else
    (void)lhs;
    (void)rhs;
    return BitValue::BinFHEPlaceholder();
#endif
}

BitValue BinFHEContext::And(const BitValue& lhs, const BitValue& rhs) const {
    assert(enabled_);
#if PRIFHETE_ENABLE_OPENFHE
    assert(impl_ != nullptr);
    return impl_->And(lhs, rhs);
#else
    (void)lhs;
    (void)rhs;
    return BitValue::BinFHEPlaceholder();
#endif
}

BitValue BinFHEContext::Or(const BitValue& lhs, const BitValue& rhs) const {
    assert(enabled_);
#if PRIFHETE_ENABLE_OPENFHE
    assert(impl_ != nullptr);
    return impl_->Or(lhs, rhs);
#else
    (void)lhs;
    (void)rhs;
    return BitValue::BinFHEPlaceholder();
#endif
}

BitValue BinFHEContext::Not(const BitValue& input) const {
    assert(enabled_);
#if PRIFHETE_ENABLE_OPENFHE
    assert(impl_ != nullptr);
    return impl_->Not(input);
#else
    (void)input;
    return BitValue::BinFHEPlaceholder();
#endif
}

}  // namespace prifhete
