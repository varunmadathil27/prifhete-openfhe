#include "wkenc/wkenc.h"

namespace prifhete {

CiphertextBytes WKEnc::Encrypt(const ByteString& plaintext,
                               const PrivateAccountPK& public_key) const {
    (void)plaintext;
    (void)public_key;

    // TODO(prifhete): Implement the paper's weakly key-homomorphic encryption.
    return CiphertextBytes{};
}

ByteString WKEnc::Decrypt(const CiphertextBytes& ciphertext,
                          const PrivateAccountSK& secret_key) const {
    (void)ciphertext;
    (void)secret_key;

    // TODO(prifhete): Implement decryption for the selected WKEnc construction.
    return ByteString{};
}

}  // namespace prifhete
