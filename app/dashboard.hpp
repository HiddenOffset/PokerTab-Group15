// ============================================================================
// dashboard.hpp — the one screen of Poker Tab (boundary class)
// ----------------------------------------------------------------------------
// Owner: Nishanth Mahendran (naming contract).
// Holds the current session and draws the New Session dialog or the
// dashboard each frame. Every change goes through core and is saved at
// once (REQ-2, REQ-4). Handlers map to use cases:
//   onCreateSession  Create Session   (REQ-1)
//   onOpenRecent     Create Session   (REQ-3)
//   onAddPlayer      Add Player       (REQ-5, REQ-6)
//   onBuyIn          Record Buy-in    (REQ-9)   planned
//   onCashOut        Record Cash-out  (REQ-14)  planned
//   onUndo           Corrections      (REQ-22)  planned
//   onSettle         Settle Up        (REQ-16)  planned
// ============================================================================
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "pokertab/session.hpp"

namespace pokertab {

class Dashboard {
public:
    Dashboard();

    // Called once per frame inside an ImGui window with a menu bar.
    void draw(double nowSeconds);

    // ---- use case handlers -------------------------------------------------
    void onCreateSession(const std::string& name, const std::string& defaultBuyIn);
    void onOpenRecent(const std::filesystem::path& path);
    void onAddPlayer(const std::string& name);
    void onBuyIn(int playerId, const std::string& amount);    // planned
    void onCashOut(int playerId, const std::string& amount);  // planned
    void onUndo();                                            // planned
    void onSettle();                                          // planned

    bool hasSession() const { return session_.has_value(); }

private:
    void persist();
    void showStatus(std::string text);
    void refreshRecent();

    void drawNewSessionDialog();
    void drawHeader();
    void drawRoster();
    void drawSidePanel();

    std::optional<Session> session_;
    std::filesystem::path sessionPath_;

    // new-session dialog
    std::string newName_;
    std::string newDefaultBuyIn_ = "20.00";
    std::string newError_;
    std::vector<std::filesystem::path> recent_;

    // dashboard
    std::string playerInput_;
    std::string playerError_;
    std::string status_;        // confirmation strip (REQ-21)
    double statusUntil_ = 0;    // clock time when the strip clears
    double now_ = 0;
    bool focusPlayerInput_ = false;
};

}  // namespace pokertab
