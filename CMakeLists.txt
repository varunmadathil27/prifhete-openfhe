cmake_minimum_required(VERSION 3.16)

project(prifhete_openfhe LANGUAGES CXX)

option(PRIFHETE_ENABLE_OPENFHE "Enable OpenFHE-backed components" OFF)
option(PRIFHETE_BUILD_TESTS "Build tests" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(prifhete
    src/state/plaintext_model.cpp
    src/protocol/key_generation.cpp
    src/protocol/registration.cpp
    src/protocol/mint.cpp
    src/protocol/transfer.cpp
    src/protocol/process_transaction.cpp
    src/protocol/update_ciphertext.cpp
    src/proof/proof_api.cpp
    src/fhe/circuits.cpp
    src/fhe/binfhe_context.cpp
    src/wkenc/wkenc.cpp
    src/commit/commitment.cpp
    src/prf/prf.cpp
    src/merkle/merkle_tree.cpp
)

target_include_directories(prifhete
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(prifhete PUBLIC cxx_std_17)

if(PRIFHETE_ENABLE_OPENFHE)
    target_compile_definitions(prifhete PUBLIC PRIFHETE_ENABLE_OPENFHE=1)
else()
    target_compile_definitions(prifhete PUBLIC PRIFHETE_ENABLE_OPENFHE=0)
endif()

if(PRIFHETE_BUILD_TESTS)
    enable_testing()

    add_executable(prifhete_smoke_test tests/test_smoke.cpp)
    target_link_libraries(prifhete_smoke_test PRIVATE prifhete)
    add_test(NAME prifhete_smoke_test COMMAND prifhete_smoke_test)

    add_executable(prifhete_plaintext_model_test tests/test_plaintext_model.cpp)
    target_link_libraries(prifhete_plaintext_model_test PRIVATE prifhete)
    add_test(NAME prifhete_plaintext_model_test COMMAND prifhete_plaintext_model_test)

    add_executable(prifhete_bit_circuits_test tests/test_bit_circuits.cpp)
    target_link_libraries(prifhete_bit_circuits_test PRIVATE prifhete)
    add_test(NAME prifhete_bit_circuits_test COMMAND prifhete_bit_circuits_test)
endif()
