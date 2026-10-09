// ============================================================================
// player.hpp — one person at the table and their money
// ----------------------------------------------------------------------------
// Owner: David Martindale (naming contract).
// ============================================================================
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "pokertab/money.hpp"
#include "pokertab/transaction.hpp"

namespace pokertab {

struct Player {
    int id = 0;
    std::string name;
    std::vector<Transaction> transactions;  // buy-ins only
    std::optional<Cents> cashOutCents;

    Cents totalInCents() const;
    int buyInCount() const { return static_cast<int>(transactions.size()); }
    bool cashedOut() const { return cashOutCents.has_value(); }
    // cash-out minus total in; 0 until cashed out
    Cents netCents() const;
};

}  // namespace pokertab
