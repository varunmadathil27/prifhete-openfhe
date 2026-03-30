#ifndef PRIFHETE_WKENC_WKENC_H
#define PRIFHETE_WKENC_WKENC_H

#include "common/types.h"

namespace prifhete {

class WKEnc {
public:
    CiphertextBytes Encrypt(const ByteString& plaintext,
                            const PrivateAccountPK& public_key) const;
    ByteString Decrypt(const CiphertextBytes& ciphertext,
                       const PrivateAccountSK& secret_key) const;
};

}  // namespace prifhete

#endif  // PRIFHETE_WKENC_WKENC_H
