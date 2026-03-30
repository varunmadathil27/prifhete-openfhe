#include "fhe/binfhe_context.h"

namespace prifhete {

BinFHEContext::BinFHEContext() = default;

bool BinFHEContext::enabled() const {
    return enabled_;
}

Status BinFHEContext::Initialize() {
#if PRIFHETE_ENABLE_OPENFHE
    enabled_ = true;
    return UnimplementedStatus("OpenFHE integration is not implemented");
#else
    enabled_ = false;
    return UnimplementedStatus("OpenFHE support is disabled");
#endif
}

}  // namespace prifhete
