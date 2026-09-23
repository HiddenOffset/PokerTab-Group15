// ============================================================================
// session.cpp
// ============================================================================
#include "pokertab/session.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>

namespace pokertab {

// ---- helpers ---------------------------------------------------------------

std::string trimmed(std::string_view s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.remove_prefix(1);
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.remove_suffix(1);
    return std::string(s);
}

static bool equalsIgnoreCase(std::string_view a, std::string_view b) {
    return a.size() == b.size() &&
           std::equal(a.begin(), a.end(), b.begin(), [](char x, char y) {
               return std::tolower(static_cast<unsigned char>(x)) ==
                      std::tolower(static_cast<unsigned char>(y));
           });
}

std::string todayIsoDate() {
    const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &now);
#else
    localtime_r(&now, &local);
#endif
    char buf[11];
    std::strftime(buf, sizeof buf, "%Y-%m-%d", &local);
    return buf;
}

// ---- Player ----------------------------------------------------------------

Cents Player::totalInCents() const {
    Cents total = 0;
    for (const auto& t : transactions) total += t.amountCents;
    return total;
}

Cents Player::netCents() const {
    return cashOutCents ? *cashOutCents - totalInCents() : 0;
}

// ---- Session ---------------------------------------------------------------

Session Session::create(std::string name) {
    return create(std::move(name), todayIsoDate());
}

Session Session::create(std::string name, std::string date) {
    Session s;
    s.name_ = trimmed(name);
    s.date_ = std::move(date);
    return s;
}

std::optional<AddPlayerError> Session::addPlayer(std::string_view rawName) {
    const std::string name = trimmed(rawName);
    if (name.empty()) return AddPlayerError::EmptyName;
    for (const auto& p : players_) {
        if (equalsIgnoreCase(p.name, name)) return AddPlayerError::DuplicateName;
    }
    Player p;
    p.id = nextPlayerId_++;
    p.name = name;
    players_.push_back(std::move(p));
    return std::nullopt;
}

const Player* Session::findPlayer(int id) const {
    for (const auto& p : players_) if (p.id == id) return &p;
    return nullptr;
}

Player* Session::findPlayer(int id) {
    for (auto& p : players_) if (p.id == id) return &p;
    return nullptr;
}

Cents Session::potCents() const {
    Cents pot = 0;
    for (const auto& p : players_) pot += p.totalInCents();
    return pot;
}

}  // namespace pokertab
