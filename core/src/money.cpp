// ============================================================================
// money.cpp
// ============================================================================
#include "pokertab/money.hpp"

#include <cctype>
#include <cstdlib>

namespace pokertab {

std::optional<Cents> parseCents(std::string_view text) {
    // trim
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) text.remove_prefix(1);
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) text.remove_suffix(1);
    if (!text.empty() && text.front() == '$') text.remove_prefix(1);
    if (text.empty()) return std::nullopt;

    Cents dollars = 0;
    Cents cents = 0;
    std::size_t i = 0;

    // whole dollars
    bool sawDigit = false;
    for (; i < text.size() && std::isdigit(static_cast<unsigned char>(text[i])); ++i) {
        if (dollars > 99'999'999'999) return std::nullopt;  // overflow guard
        dollars = dollars * 10 + (text[i] - '0');
        sawDigit = true;
    }

    // optional fraction, at most two digits
    if (i < text.size() && text[i] == '.') {
        ++i;
        int fracDigits = 0;
        for (; i < text.size() && std::isdigit(static_cast<unsigned char>(text[i])); ++i) {
            if (++fracDigits > 2) return std::nullopt;
            cents = cents * 10 + (text[i] - '0');
            sawDigit = true;
        }
        if (fracDigits == 1) cents *= 10;
    }

    if (!sawDigit || i != text.size()) return std::nullopt;
    return dollars * 100 + cents;
}

std::string formatCents(Cents cents) {
    const bool negative = cents < 0;
    if (negative) cents = -cents;
    std::string s = std::to_string(cents / 100) + ".";
    const Cents frac = cents % 100;
    if (frac < 10) s += '0';
    s += std::to_string(frac);
    return (negative ? "-$" : "$") + s;
}

}  // namespace pokertab
