#include <cassert>
#include <cstdint>
#include <string>

#include "state/plaintext_model.h"

namespace {

using prifhete::ByteString;
using prifhete::MerkleRoot;
using prifhete::PRFOutput;
using prifhete::PlaintextModel;

ByteString BytesFromString(const std::string& value) {
    return ByteString(value.begin(), value.end());
}

PRFOutput MakePrfOutput(const std::string& value) {
    return PRFOutput{BytesFromString(value)};
}

std::uint64_t BalanceOf(const PlaintextModel& model,
                        PlaintextModel::AccountIndex account_index) {
    std::uint64_t balance = 0;
    const bool found = model.TryGetBalance(account_index, &balance);
    assert(found);
    return balance;
}

PlaintextModel BuildRegisteredModel(PlaintextModel::AccountIndex* alice_index,
                                    PlaintextModel::AccountIndex* bob_index) {
    PlaintextModel model;

    const PlaintextModel::RegistrationResult alice =
        model.Register(PlaintextModel::RegistrationRequest{"alice", {}});
    assert(alice.status.ok);

    const PlaintextModel::RegistrationResult bob =
        model.Register(PlaintextModel::RegistrationRequest{"bob", {}});
    assert(bob.status.ok);

    if (alice_index != nullptr) {
        *alice_index = alice.account_index;
    }
    if (bob_index != nullptr) {
        *bob_index = bob.account_index;
    }
    return model;
}

void TestValidRegistration() {
    PlaintextModel model;

    const PlaintextModel::RegistrationResult registration =
        model.Register(PlaintextModel::RegistrationRequest{"alice", {}});

    assert(registration.status.ok);
    assert(model.account_count() == 1);
    assert(model.IsValidAccountIndex(registration.account_index));
    assert(model.HasAccount("alice"));

    const auto alice_index = model.FindAccountIndex("alice");
    assert(alice_index.has_value());
    assert(alice_index->value == registration.account_index.value);

    const prifhete::PrivateAccountEntry* account =
        model.FindAccount(registration.account_index);
    assert(account != nullptr);
    assert(account->account_id == "alice");
    assert(account->epoch == model.current_epoch());
    assert(account->inclusion_root.bytes == registration.root.bytes);
}

void TestValidMint() {
    PlaintextModel::AccountIndex alice_index;
    PlaintextModel model = BuildRegisteredModel(&alice_index, nullptr);

    const PlaintextModel::MintResult mint = model.Mint(
        PlaintextModel::PublicToPrivateMint{
            "mint-1",
            alice_index,
            25,
            MakePrfOutput("mint-prf-1"),
            model.current_root(),
        });

    assert(mint.status.ok);
    assert(mint.root.bytes == model.current_root().bytes);
    assert(BalanceOf(model, alice_index) == 25);
    assert(model.HasSeenPrfOutputInCurrentEpoch(MakePrfOutput("mint-prf-1")));
}

void TestValidTransfer() {
    PlaintextModel::AccountIndex alice_index;
    PlaintextModel::AccountIndex bob_index;
    PlaintextModel model = BuildRegisteredModel(&alice_index, &bob_index);

    const PlaintextModel::MintResult mint = model.Mint(
        PlaintextModel::PublicToPrivateMint{
            "mint-1",
            alice_index,
            40,
            MakePrfOutput("mint-prf-1"),
            model.current_root(),
        });
    assert(mint.status.ok);

    const PlaintextModel::TransferResult transfer = model.Transfer(
        PlaintextModel::PrivateTransfer{
            "transfer-1",
            alice_index,
            bob_index,
            15,
            MakePrfOutput("transfer-prf-1"),
            model.current_root(),
        });

    assert(transfer.status.ok);
    assert(transfer.root.bytes == model.current_root().bytes);
    assert(BalanceOf(model, alice_index) == 25);
    assert(BalanceOf(model, bob_index) == 15);
}

void TestOverspendRejection() {
    PlaintextModel::AccountIndex alice_index;
    PlaintextModel::AccountIndex bob_index;
    PlaintextModel model = BuildRegisteredModel(&alice_index, &bob_index);

    const PlaintextModel::MintResult mint = model.Mint(
        PlaintextModel::PublicToPrivateMint{
            "mint-1",
            alice_index,
            10,
            MakePrfOutput("mint-prf-1"),
            model.current_root(),
        });
    assert(mint.status.ok);

    const PlaintextModel::TransferResult transfer = model.Transfer(
        PlaintextModel::PrivateTransfer{
            "transfer-overspend",
            alice_index,
            bob_index,
            11,
            MakePrfOutput("transfer-prf-overspend"),
            model.current_root(),
        });

    assert(!transfer.status.ok);
    assert(transfer.status.message == "overspend rejected");
    assert(BalanceOf(model, alice_index) == 10);
    assert(BalanceOf(model, bob_index) == 0);
    assert(!model.HasSeenPrfOutputInCurrentEpoch(MakePrfOutput("transfer-prf-overspend")));
}

void TestDuplicatePrfOutputRejection() {
    PlaintextModel::AccountIndex alice_index;
    PlaintextModel model = BuildRegisteredModel(&alice_index, nullptr);

    const PRFOutput duplicate_prf = MakePrfOutput("duplicate-prf");
    const PlaintextModel::MintResult first = model.Mint(
        PlaintextModel::PublicToPrivateMint{
            "mint-1",
            alice_index,
            10,
            duplicate_prf,
            model.current_root(),
        });
    assert(first.status.ok);

    const PlaintextModel::MintResult second = model.Mint(
        PlaintextModel::PublicToPrivateMint{
            "mint-2",
            alice_index,
            5,
            duplicate_prf,
            model.current_root(),
        });

    assert(!second.status.ok);
    assert(second.status.message == "duplicate PRFOutput in current epoch");
    assert(BalanceOf(model, alice_index) == 10);
}

void TestStaleRootRejection() {
    PlaintextModel::AccountIndex alice_index;
    PlaintextModel::AccountIndex bob_index;
    PlaintextModel model = BuildRegisteredModel(&alice_index, &bob_index);

    const MerkleRoot old_root = model.current_root();

    const prifhete::Status epoch_one = model.AdvanceEpoch();
    assert(epoch_one.ok);
    const prifhete::Status epoch_two = model.AdvanceEpoch();
    assert(epoch_two.ok);

    const PlaintextModel::MintResult stale_mint = model.Mint(
        PlaintextModel::PublicToPrivateMint{
            "mint-stale",
            alice_index,
            7,
            MakePrfOutput("stale-root-prf"),
            old_root,
        });

    assert(!stale_mint.status.ok);
    assert(stale_mint.status.message == "referenced_root is stale");
    assert(BalanceOf(model, alice_index) == 0);
}

void TestReceiverBalanceUpdate() {
    PlaintextModel::AccountIndex alice_index;
    PlaintextModel::AccountIndex bob_index;
    PlaintextModel model = BuildRegisteredModel(&alice_index, &bob_index);

    assert(model.Mint(PlaintextModel::PublicToPrivateMint{
        "mint-1",
        alice_index,
        50,
        MakePrfOutput("receiver-update-mint"),
        model.current_root(),
    }).status.ok);

    const std::uint64_t before = BalanceOf(model, bob_index);
    const PlaintextModel::TransferResult transfer = model.Transfer(
        PlaintextModel::PrivateTransfer{
            "transfer-1",
            alice_index,
            bob_index,
            18,
            MakePrfOutput("receiver-update-transfer"),
            model.current_root(),
        });

    assert(transfer.status.ok);
    assert(BalanceOf(model, bob_index) == before + 18);
    assert(transfer.receiver_balance == before + 18);
}

void TestSenderBalanceDecrease() {
    PlaintextModel::AccountIndex alice_index;
    PlaintextModel::AccountIndex bob_index;
    PlaintextModel model = BuildRegisteredModel(&alice_index, &bob_index);

    assert(model.Mint(PlaintextModel::PublicToPrivateMint{
        "mint-1",
        alice_index,
        35,
        MakePrfOutput("sender-decrease-mint"),
        model.current_root(),
    }).status.ok);

    const std::uint64_t before = BalanceOf(model, alice_index);
    const PlaintextModel::TransferResult transfer = model.Transfer(
        PlaintextModel::PrivateTransfer{
            "transfer-1",
            alice_index,
            bob_index,
            9,
            MakePrfOutput("sender-decrease-transfer"),
            model.current_root(),
        });

    assert(transfer.status.ok);
    assert(BalanceOf(model, alice_index) == before - 9);
    assert(transfer.sender_balance == before - 9);
}

void TestEpochAdvanceBehavior() {
    PlaintextModel::AccountIndex alice_index;
    PlaintextModel model = BuildRegisteredModel(&alice_index, nullptr);

    const PlaintextModel::MintResult first_mint = model.Mint(
        PlaintextModel::PublicToPrivateMint{
            "mint-1",
            alice_index,
            12,
            MakePrfOutput("reused-next-epoch"),
            model.current_root(),
        });
    assert(first_mint.status.ok);

    const PlaintextModel::EpochStateView before_advance = model.InspectEpochState();
    const MerkleRoot old_root = model.current_root();

    const prifhete::Status advance_status = model.AdvanceEpoch();
    assert(advance_status.ok);

    const PlaintextModel::EpochStateView after_advance = model.InspectEpochState();
    assert(after_advance.current_epoch == before_advance.current_epoch + 1);
    assert(after_advance.root_version == before_advance.root_version + 1);
    assert(after_advance.current_root.bytes != old_root.bytes);
    assert(after_advance.used_prf_output_count == 0);
    assert(model.IsValidRoot(old_root));

    const PlaintextModel::MintResult second_mint = model.Mint(
        PlaintextModel::PublicToPrivateMint{
            "mint-2",
            alice_index,
            3,
            MakePrfOutput("reused-next-epoch"),
            model.current_root(),
        });

    assert(second_mint.status.ok);
    assert(BalanceOf(model, alice_index) == 15);
}

void TestInvalidAccountIndexRejection() {
    PlaintextModel::AccountIndex alice_index;
    PlaintextModel model = BuildRegisteredModel(&alice_index, nullptr);

    const PlaintextModel::AccountIndex invalid_index{999};
    assert(!model.IsValidAccountIndex(invalid_index));

    const PlaintextModel::MintResult mint = model.Mint(
        PlaintextModel::PublicToPrivateMint{
            "mint-invalid",
            invalid_index,
            1,
            MakePrfOutput("invalid-index-prf"),
            model.current_root(),
        });

    assert(!mint.status.ok);
    assert(mint.status.message == "invalid recipient account index");
}

}  // namespace

int main() {
    TestValidRegistration();
    TestValidMint();
    TestValidTransfer();
    TestOverspendRejection();
    TestDuplicatePrfOutputRejection();
    TestStaleRootRejection();
    TestReceiverBalanceUpdate();
    TestSenderBalanceDecrease();
    TestEpochAdvanceBehavior();
    TestInvalidAccountIndexRejection();
    return 0;
}
