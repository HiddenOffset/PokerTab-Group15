// ============================================================================
// money.hpp — integer-cent money type
// ----------------------------------------------------------------------------
// All amounts are stored as int64 cents (RAD Appendix B). No floating point
// anywhere in the accounting path.
// ============================================================================
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace pokertab {

using Cents = std::int64_t;

// "20", "20.5", "20.50", "$20.50" -> 2050. Rejects negatives, blank input,
// non-numeric text, and more than two decimal places (REQ-11).
std::optional<Cents> parseCents(std::string_view text);

// 2050 -> "$20.50"; -1500 -> "-$15.00"
std::string formatCents(Cents cents);

}  // namespace pokertab
