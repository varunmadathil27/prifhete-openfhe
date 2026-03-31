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

## Plaintext Oracle Model

`src/state/plaintext_model.*` now acts as the deterministic reference oracle for
future encrypted tests. It is intentionally not private and intentionally not
cryptographic.

The plaintext model currently supports:

- registration into an indexed private-account set
- public-to-private mint into a registered account
- private transfer between registered accounts
- explicit epoch advancement
- duplicate `PRFOutput` rejection within the current epoch
- typed placeholder root-version tracking for transaction references

Transactions reference a strongly typed root handle containing:

- `MerkleRoot root`
- `EpochNumber epoch`
- `RootVersion version`

The `MerkleRoot.bytes` payload is still a deterministic placeholder string:

- `plaintext-root:e<epoch>:v<root_version>`

This is not Merkle cryptography. It only models root identity and evolution so
tests can assert correct root freshness and epoch-bound validity before the real
tree and authentication path logic exist.

### State Rules

- Each registered account receives a stable `AccountIndex`.
- Balances are stored in plaintext and updated exactly by mint and transfer.
- Every successful state transition, including epoch advance, produces a new
  placeholder root version.
- The oracle tracks every root generated in the current epoch so transactions
  can reference any current-epoch root version, not only the latest one.
- Roots remain valid only inside the configured epoch window used by the oracle.
- A `PRFOutput` may be used at most once per epoch.
- Failed transactions do not mutate balances, root state, or `PRFOutput`
  replay state.
- Root references are rejected if the root is unknown, stale, or carries
  mismatched epoch/version metadata for the encoded placeholder root.
- Invalid account references and overspends are rejected before state mutation.

## Current Status

The repository still preserves paper-level module boundaries while using
placeholder cryptographic components. The plaintext model is the executable
oracle for ledger semantics; the protocol-layer registration, mint, and
transfer wrappers are still stubs and should later be connected to the paper's
real cryptographic algorithms without changing the basic state invariants above.

TODO: Implement the paper algorithms against real cryptographic constructions
while preserving the source-file boundaries captured in
`docs/paper_to_code_map.md`.
