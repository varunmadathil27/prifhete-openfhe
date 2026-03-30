#include "commit/commitment.h"

namespace prifhete {

CommitmentBytes CommitmentScheme::Commit(const ByteString& message) const {
    (void)message;

    // TODO(prifhete): Implement the paper-compatible commitment construction.
    return CommitmentBytes{};
}

bool CommitmentScheme::Verify(const ByteString& message,
                              const CommitmentBytes& commitment) const {
    (void)message;
    (void)commitment;

    // TODO(prifhete): Implement commitment verification.
    return false;
}

}  // namespace prifhete
