#ifndef PRIFHETE_FHE_BINFHE_CONTEXT_H
#define PRIFHETE_FHE_BINFHE_CONTEXT_H

#include "common/types.h"

namespace prifhete {

class BinFHEContext {
public:
    BinFHEContext();

    bool enabled() const;
    Status Initialize();

private:
    bool enabled_ = false;
};

}  // namespace prifhete

#endif  // PRIFHETE_FHE_BINFHE_CONTEXT_H
