#ifndef PRIFHETE_FHE_BINFHE_CONTEXT_H
#define PRIFHETE_FHE_BINFHE_CONTEXT_H

#include "common/types.h"
#include "fhe/bit_value.h"

namespace prifhete {

class BinFHEContext final : public BitBackend {
public:
    BinFHEContext();

    bool enabled() const;
    Status Initialize();

    BitBackendKind kind() const override;
    BitValue Constant(bool value) const override;
    BitValue Xor(const BitValue& lhs, const BitValue& rhs) const override;
    BitValue And(const BitValue& lhs, const BitValue& rhs) const override;
    BitValue Or(const BitValue& lhs, const BitValue& rhs) const override;
    BitValue Not(const BitValue& input) const override;

private:
    bool enabled_ = false;
};

}  // namespace prifhete

#endif  // PRIFHETE_FHE_BINFHE_CONTEXT_H
