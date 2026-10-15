// ============================================================================
// session.cpp
// ============================================================================
#include "pokertab/session.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <limits>

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

    //get the local timestamp for each recorded transaction
    static std::string currentTimestamp()
    {
        const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm local{};
#ifdef _WIN32
        localtime_s(&local, &now);
#else
        localtime_r(&now, &local);
#endif
        char buffer[20];
        std::strftime(buffer, sizeof buffer, "%Y-%m-%dT%H:%M:%S", &local);
        return buffer;
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

    std::optional<RemovePlayerError> Session::removePlayer(int playerId) {
        auto playerIt = std::find_if(
            players_.begin(),
            players_.end(),
            [playerId](const Player& player) {
                return player.id == playerId;
            }
        );

        if (playerIt == players_.end()) {
            return RemovePlayerError::PlayerNotFound;
        }

        if (!playerIt->transactions.empty()) {
            return RemovePlayerError::HasTransactions;
        }

        players_.erase(playerIt);
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

    //validate before updating the transaction history or its next id
    std::optional<BuyInError> Session::recordBuyIn(int playerId, std::string_view amount)
    {
        Player* player = findPlayer(playerId);
        if (player == nullptr)
        {
            return BuyInError::PlayerNotFound;
        }

        const auto cents = parseCents(amount);
        if (!cents || *cents <= 0)
        {
            return BuyInError::InvalidAmount;
        }

        //prevent integer overflow when totals are recalculated
        const Cents maxCents = std::numeric_limits<Cents>::max();
        if (*cents > maxCents - player->totalInCents() ||
            *cents > maxCents - potCents())
        {
            return BuyInError::AmountTooLarge;
        }

        const Transaction transaction{ nextTransactionId_, currentTimestamp(), *cents };
        player->transactions.push_back(transaction);
        ++nextTransactionId_;
        return std::nullopt;
    }

    Cents Session::potCents() const {
        Cents pot = 0;
        for (const auto& p : players_) pot += p.totalInCents();
        return pot;
    }

}  // namespace pokertab
