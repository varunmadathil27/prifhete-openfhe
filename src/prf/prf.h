#ifndef PRIFHETE_PRF_PRF_H
#define PRIFHETE_PRF_PRF_H

#include "common/types.h"

namespace prifhete {

class PRF {
public:
    PRFOutput Evaluate(const ByteString& key, const ByteString& input) const;
};

}  // namespace prifhete

#endif  // PRIFHETE_PRF_PRF_H
