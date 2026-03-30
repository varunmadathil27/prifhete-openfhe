# Architecture

This repository is organized to mirror the PriFHEte paper's protocol objects and
algorithm boundaries before any cryptographic backend is integrated.

## Design Principles

- Preserve paper-level objects at the type level.
- Keep protocol phases separated instead of collapsing them into a single-key or
  packed ledger abstraction.
- Allow the project to compile with placeholder implementations when
  `PRIFHETE_ENABLE_OPENFHE=OFF`.
- Avoid implying correctness for any unimplemented cryptographic step.

## Module Layout

- `src/common`: shared protocol types and byte-string wrappers
- `src/state`: plaintext state model used for development scaffolding
- `src/protocol`: orchestration entry points for paper algorithms
- `src/proof`: placeholder zero-knowledge proof API surface
- `src/fhe`: placeholder FHE context boundary
- `src/wkenc`: weakly key-homomorphic encryption placeholder boundary
- `src/commit`: commitment placeholder boundary
- `src/prf`: PRF placeholder boundary
- `src/merkle`: Merkle tree placeholder boundary

## Current Status

The current scaffold only defines interfaces, placeholder containers, and minimal
stub behavior needed for compilation and smoke testing.

TODO: Implement the paper algorithms against real cryptographic constructions
while preserving the source-file boundaries captured in
`docs/paper_to_code_map.md`.
