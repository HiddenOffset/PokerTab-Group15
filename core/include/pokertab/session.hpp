// ============================================================================
// session.hpp — one poker session: players and their money
// ----------------------------------------------------------------------------
// Owner: David Martindale (naming contract).
// Implements REQ-1 (create session) and REQ-5 / REQ-6 (add player, reject
// duplicates). Buy-ins, cash-outs and settlement land in later REQs; the
// data shape for them (Appendix B) is already here so those PRs only add
// methods.
// ============================================================================
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "pokertab/money.hpp"
#include "pokertab/player.hpp"

namespace pokertab {

enum class AddPlayerError { EmptyName, DuplicateName };

class Session {
public:
    // REQ-1: new empty session with a user-supplied name and the current date.
    static Session create(std::string name);
    // Same, with an explicit YYYY-MM-DD date (tests, reloads).
    static Session create(std::string name, std::string date);

    const std::string& name() const { return name_; }
    const std::string& date() const { return date_; }
    Cents defaultBuyInCents() const { return defaultBuyInCents_; }
    void setDefaultBuyInCents(Cents c) { defaultBuyInCents_ = c; }

    const std::vector<Player>& players() const { return players_; }
    std::vector<Player>& players() { return players_; }

    // REQ-5 / REQ-6: add a player by name. Names are trimmed; comparison is
    // case-insensitive. Returns std::nullopt on success (the player is
    // players().back()), or the reason it was rejected.
    std::optional<AddPlayerError> addPlayer(std::string_view name);

    const Player* findPlayer(int id) const;
    Player* findPlayer(int id);

    // REQ-26: sum of every buy-in in the session
    Cents potCents() const;

    // Next ids handed out by the session; persisted so reloads stay unique.
    int nextPlayerId() const { return nextPlayerId_; }
    int nextTransactionId() const { return nextTransactionId_; }

private:
    friend class SessionStore;
    std::string name_;
    std::string date_;
    Cents defaultBuyInCents_ = 2000;
    std::vector<Player> players_;
    int nextPlayerId_ = 1;
    int nextTransactionId_ = 1;
};

// YYYY-MM-DD in local time
std::string todayIsoDate();
// trims leading/trailing whitespace
std::string trimmed(std::string_view s);

}  // namespace pokertab
