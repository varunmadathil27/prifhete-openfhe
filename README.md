# PriFHEte OpenFHE Prototype

Initial scaffold for a paper-faithful PriFHEte prototype.

This repository currently provides:

- a conservative C++17 project layout
- placeholder protocol object types that preserve the paper's boundaries
- stub implementations that compile without OpenFHE
- documentation mapping paper algorithms to source files

This repository does not yet implement real cryptography, zero-knowledge proofs,
or a production transaction pipeline.

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

To configure with explicit options:

```bash
cmake -S . -B build \
  -DPRIFHETE_ENABLE_OPENFHE=OFF \
  -DPRIFHETE_BUILD_TESTS=ON
```
