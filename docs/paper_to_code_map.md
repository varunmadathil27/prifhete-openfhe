# Paper To Code Map

This document maps paper algorithms to their intended implementation files. The
current code is scaffold-only and does not claim that any algorithm is complete.

## Algorithm Mapping

- `KeyGeneration`
  - `src/protocol/key_generation.h`
  - `src/protocol/key_generation.cpp`
- `Registration`
  - `src/protocol/registration.h`
  - `src/protocol/registration.cpp`
- `Mint`
  - `src/protocol/mint.h`
  - `src/protocol/mint.cpp`
- `Transfer`
  - `src/protocol/transfer.h`
  - `src/protocol/transfer.cpp`
- `ProcessTransaction`
  - `src/protocol/process_transaction.h`
  - `src/protocol/process_transaction.cpp`
- `UpdateCiphertext`
  - `src/protocol/update_ciphertext.h`
  - `src/protocol/update_ciphertext.cpp`
- `R_TRANSFER`
  - `src/proof/proof_api.h`
  - `src/proof/proof_api.cpp`

## Supporting Components

- FHE boundary:
  - `src/fhe/binfhe_context.h`
  - `src/fhe/binfhe_context.cpp`
- Weakly key-homomorphic encryption boundary:
  - `src/wkenc/wkenc.h`
  - `src/wkenc/wkenc.cpp`
- Commitment boundary:
  - `src/commit/commitment.h`
  - `src/commit/commitment.cpp`
- PRF boundary:
  - `src/prf/prf.h`
  - `src/prf/prf.cpp`
- Merkle boundary:
  - `src/merkle/merkle_tree.h`
  - `src/merkle/merkle_tree.cpp`
- Plaintext development model:
  - `src/state/plaintext_model.h`
  - `src/state/plaintext_model.cpp`
