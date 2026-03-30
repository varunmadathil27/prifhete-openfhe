#ifndef PRIFHETE_STATE_PLAINTEXT_MODEL_H
#define PRIFHETE_STATE_PLAINTEXT_MODEL_H

#include <string>
#include <unordered_map>

#include "common/types.h"

namespace prifhete {

class PlaintextModel {
public:
    PlaintextModel();

    bool HasAccount(const std::string& account_id) const;
    Status UpsertAccount(const PrivateAccountEntry& entry);
    const PrivateAccountEntry* FindAccount(const std::string& account_id) const;

    EpochNumber current_epoch() const;
    void set_current_epoch(EpochNumber epoch);

private:
    std::unordered_map<std::string, PrivateAccountEntry> accounts_;
    EpochNumber current_epoch_ = 0;
};

}  // namespace prifhete

#endif  // PRIFHETE_STATE_PLAINTEXT_MODEL_H
