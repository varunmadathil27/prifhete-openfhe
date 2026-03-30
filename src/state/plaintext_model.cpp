#include "state/plaintext_model.h"

namespace prifhete {

PlaintextModel::PlaintextModel() = default;

bool PlaintextModel::HasAccount(const std::string& account_id) const {
    return accounts_.find(account_id) != accounts_.end();
}

Status PlaintextModel::UpsertAccount(const PrivateAccountEntry& entry) {
    if (entry.account_id.empty()) {
        return Status{false, "account_id must not be empty"};
    }

    accounts_[entry.account_id] = entry;
    return OkStatus();
}

const PrivateAccountEntry* PlaintextModel::FindAccount(
    const std::string& account_id) const {
    const auto iter = accounts_.find(account_id);
    if (iter == accounts_.end()) {
        return nullptr;
    }
    return &iter->second;
}

EpochNumber PlaintextModel::current_epoch() const {
    return current_epoch_;
}

void PlaintextModel::set_current_epoch(EpochNumber epoch) {
    current_epoch_ = epoch;
}

}  // namespace prifhete
