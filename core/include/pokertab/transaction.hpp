// ============================================================================
// transaction.hpp — one buy-in
// ----------------------------------------------------------------------------
// Owner: Matthew Rohrer (naming contract). Cash-out is not a Transaction; it
// is a single field on Player (RAD Appendix B).
// ============================================================================
#pragma once

#include <string>

#include "pokertab/money.hpp"

namespace pokertab {

struct Transaction {
    int id = 0;
    std::string timestamp;  // ISO-8601, local time
    Cents amountCents = 0;
};

}  // namespace pokertab
