#include "prf/prf.h"

namespace prifhete {

PRFOutput PRF::Evaluate(const ByteString& key, const ByteString& input) const {
    (void)key;
    (void)input;

    // TODO(prifhete): Implement the PRF required by the paper.
    return PRFOutput{};
}

}  // namespace prifhete
