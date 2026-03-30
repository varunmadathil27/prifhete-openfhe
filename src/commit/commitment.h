#ifndef PRIFHETE_COMMIT_COMMITMENT_H
#define PRIFHETE_COMMIT_COMMITMENT_H

#include "common/types.h"

namespace prifhete {

class CommitmentScheme {
public:
    CommitmentBytes Commit(const ByteString& message) const;
    bool Verify(const ByteString& message, const CommitmentBytes& commitment) const;
};

}  // namespace prifhete

#endif  // PRIFHETE_COMMIT_COMMITMENT_H
