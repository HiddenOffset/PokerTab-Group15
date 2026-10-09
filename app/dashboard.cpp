// ============================================================================
// dashboard.cpp — Dear ImGui drawing and use case handlers
// ============================================================================
#include "dashboard.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

#include <algorithm>

#include "pokertab/money.hpp"
#include "pokertab/session_store.hpp"

namespace fs = std::filesystem;

namespace pokertab {

namespace {
const ImVec4 kErrorRed(0.85f, 0.25f, 0.25f, 1);
}

Dashboard::Dashboard() { refreshRecent(); }

// ---- helpers ---------------------------------------------------------------

void Dashboard::showStatus(std::string text) {
    status_ = std::move(text);
    statusUntil_ = now_ + 3.0;
}

void Dashboard::persist() {
    try {
        SessionStore::save(*session_, sessionPath_);
    } catch (const std::exception& e) {
        showStatus(std::string("Save failed: ") + e.what());
    }
}

void Dashboard::refreshRecent() {
    recent_.clear();
    std::error_code ec;
    for (const auto& entry : fs::directory_iterator(SessionStore::defaultSessionsDir(), ec)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") recent_.push_back(entry.path());
    }
    std::sort(recent_.begin(), recent_.end(), [](const fs::path& a, const fs::path& b) {
        return a.filename().string() > b.filename().string();  // date prefix => newest first
    });
    if (recent_.size() > 8) recent_.resize(8);
}

// ---- use case handlers -----------------------------------------------------

// Create Session (REQ-1, REQ-2)
void Dashboard::onCreateSession(const std::string& rawName, const std::string& defaultBuyIn) {
    const std::string name = trimmed(rawName);
    const auto buyIn = parseCents(defaultBuyIn);
    if (name.empty()) {
        newError_ = "Enter a session name.";
        return;
    }
    if (!buyIn || *buyIn <= 0) {
        newError_ = "Default buy-in must be a positive dollar amount, e.g. 20.00";
        return;
    }
    Session s = Session::create(name);  // today's date
    s.setDefaultBuyInCents(*buyIn);
    sessionPath_ = SessionStore::pathFor(s);
    session_ = std::move(s);
    newError_.clear();
    persist();
    showStatus("Created session \"" + name + "\"");
    focusPlayerInput_ = true;
}

// Create Session, open an existing file (REQ-3)
void Dashboard::onOpenRecent(const fs::path& path) {
    try {
        session_ = SessionStore::load(path);
        sessionPath_ = path;
        newError_.clear();
        showStatus("Opened " + path.filename().string());
    } catch (const std::exception& e) {
        newError_ = e.what();
    }
}

// Add Player (REQ-5, REQ-6)
void Dashboard::onAddPlayer(const std::string& rawName) {
    const std::string name = trimmed(rawName);
    if (auto err = session_->addPlayer(name)) {
        playerError_ = *err == AddPlayerError::EmptyName ? "Enter a player name."
                                                        : "\"" + name + "\" is already on the roster.";
        return;
    }
    playerError_.clear();
    playerInput_.clear();
    persist();
    showStatus("Added " + name + " to the roster");
    focusPlayerInput_ = true;
}

void Dashboard::onBuyIn(int, const std::string&) {}    // REQ-9, planned
void Dashboard::onCashOut(int, const std::string&) {}  // REQ-14, planned
void Dashboard::onUndo() {}                            // REQ-22, planned
void Dashboard::onSettle() {}                          // REQ-16, planned

// ---- frame -----------------------------------------------------------------

void Dashboard::draw(double nowSeconds) {
    now_ = nowSeconds;
    if (session_) {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Session")) {
                if (ImGui::MenuItem("New session...")) {
                    session_.reset();
                    newName_.clear();
                    refreshRecent();
                }
                ImGui::Separator();
                ImGui::TextDisabled("%s", sessionPath_.string().c_str());
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (!session_) return;  // reset inside the menu this frame

        drawHeader();
        ImGui::Spacing();

        const float side = 300;
        ImGui::BeginChild("left", ImVec2(ImGui::GetContentRegionAvail().x - side - 12, 0));
        drawRoster();
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("right", ImVec2(side, 0));
        drawSidePanel();
        ImGui::EndChild();
    } else {
        drawNewSessionDialog();
    }
}

// ---- New Session dialog (REQ-1, REQ-3) ------------------------------------

void Dashboard::drawNewSessionDialog() {
    const ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(560, 0));
    if (!ImGui::IsPopupOpen("New session")) ImGui::OpenPopup("New session");

    if (!ImGui::BeginPopupModal("New session", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) return;

    ImGui::TextUnformatted("Session name");
    ImGui::SetNextItemWidth(-1);
    if (ImGui::IsWindowAppearing()) ImGui::SetKeyboardFocusHere();
    bool submit = ImGui::InputTextWithHint("##name", "Friday night", &newName_, ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::TextDisabled("Required. Used as the file name.");
    ImGui::Spacing();

    ImGui::Columns(2, nullptr, false);
    ImGui::TextUnformatted("Date");
    ImGui::BeginDisabled();
    std::string today = todayIsoDate();
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##date", &today, ImGuiInputTextFlags_ReadOnly);
    ImGui::EndDisabled();
    ImGui::TextDisabled("Set automatically to today.");
    ImGui::NextColumn();
    ImGui::TextUnformatted("Default buy-in");
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##buyin", &newDefaultBuyIn_);
    ImGui::TextDisabled("Pre-fills every buy-in.");
    ImGui::Columns(1);
    ImGui::Spacing();

    if (!recent_.empty()) {
        ImGui::SeparatorText("Or open a recent session");
        for (const auto& p : recent_) {
            ImGui::TextUnformatted(p.filename().string().c_str());
            ImGui::SameLine(ImGui::GetWindowWidth() - 70);
            ImGui::PushID(p.string().c_str());
            if (ImGui::SmallButton("Open")) {
                onOpenRecent(p);
                if (session_) ImGui::CloseCurrentPopup();
            }
            ImGui::PopID();
        }
        ImGui::Spacing();
    }

    if (!newError_.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Text, kErrorRed);
        ImGui::TextWrapped("%s", newError_.c_str());
        ImGui::PopStyleColor();
    }

    ImGui::Separator();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 160);
    if (ImGui::Button("Create session", ImVec2(160, 36)) || submit) {
        onCreateSession(newName_, newDefaultBuyIn_);
        if (session_) ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
}

// ---- Dashboard (REQ-5, REQ-24, REQ-25, REQ-26) -----------------------------

void Dashboard::drawHeader() {
    const Session& s = *session_;
    ImGui::BeginChild("header", ImVec2(0, 78), ImGuiChildFlags_Borders);
    ImGui::TextDisabled("SESSION");
    ImGui::SetWindowFontScale(1.6f);
    ImGui::TextUnformatted(s.name().c_str());
    ImGui::SetWindowFontScale(1.0f);

    ImGui::SameLine(320);
    ImGui::BeginGroup();
    ImGui::TextDisabled("DATE");
    ImGui::TextUnformatted(s.date().c_str());
    ImGui::EndGroup();

    ImGui::SameLine(480);
    ImGui::BeginGroup();
    ImGui::TextDisabled("PLAYERS");
    ImGui::Text("%zu", s.players().size());
    ImGui::EndGroup();

    ImGui::SameLine(ImGui::GetWindowWidth() - 180);
    ImGui::BeginGroup();
    ImGui::TextDisabled("TOTAL POT");
    ImGui::SetWindowFontScale(1.6f);
    ImGui::TextUnformatted(formatCents(s.potCents()).c_str());
    ImGui::SetWindowFontScale(1.0f);
    ImGui::EndGroup();
    ImGui::EndChild();
}

void Dashboard::drawRoster() {
    const Session& s = *session_;
    ImGui::TextDisabled("ROSTER");
    const ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                                  ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp;
    const float tableHeight = ImGui::GetContentRegionAvail().y - 110;
    if (ImGui::BeginTable("roster", 6, flags, ImVec2(0, tableHeight))) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 2.0f);
        ImGui::TableSetupColumn("Buy-ins");
        ImGui::TableSetupColumn("Total in");
        ImGui::TableSetupColumn("Cash-out");
        ImGui::TableSetupColumn("Net");
        ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthStretch, 1.6f);
        ImGui::TableHeadersRow();

        for (const Player& p : s.players()) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::TextUnformatted(p.name.c_str());
            ImGui::TableNextColumn(); ImGui::Text("%d", p.buyInCount());
            ImGui::TableNextColumn(); ImGui::TextUnformatted(formatCents(p.totalInCents()).c_str());
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(p.cashedOut() ? formatCents(*p.cashOutCents).c_str() : "-");
            ImGui::TableNextColumn(); ImGui::TextUnformatted(formatCents(p.netCents()).c_str());
            ImGui::TableNextColumn();
            ImGui::PushID(p.id);
            ImGui::BeginDisabled();
            ImGui::SmallButton("Buy-in");
            ImGui::SameLine();
            ImGui::SmallButton("Cash-out");
            ImGui::EndDisabled();
            ImGui::PopID();
        }
        if (s.players().empty()) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextDisabled("No players yet. Add the first one below.");
        }
        ImGui::EndTable();
    }

    // REQ-5: add player
    ImGui::Spacing();
    ImGui::TextUnformatted("Add player");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(260);
    if (focusPlayerInput_) {
        ImGui::SetKeyboardFocusHere();
        focusPlayerInput_ = false;
    }
    const bool enter = ImGui::InputTextWithHint("##player", "Name", &playerInput_, ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    if (ImGui::Button("Add Player", ImVec2(120, 0)) || enter) onAddPlayer(playerInput_);
    if (!playerError_.empty()) {
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, kErrorRed);
        ImGui::TextUnformatted(playerError_.c_str());
        ImGui::PopStyleColor();
    }

    // REQ-21 confirmation strip + undo placeholder
    ImGui::Spacing();
    ImGui::BeginChild("status", ImVec2(0, 40), ImGuiChildFlags_Borders);
    if (now_ < statusUntil_) ImGui::TextUnformatted(status_.c_str());
    else ImGui::TextDisabled("Confirmations appear here for 3 seconds.");
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    ImGui::BeginDisabled();
    ImGui::Button("Undo last");
    ImGui::EndDisabled();
    ImGui::EndChild();
}

void Dashboard::drawSidePanel() {
    const Session& s = *session_;
    ImGui::TextDisabled("AMOUNT");
    ImGui::BeginChild("amount", ImVec2(0, 130), ImGuiChildFlags_Borders);
    ImGui::BeginDisabled();
    std::string amount = formatCents(s.defaultBuyInCents()).substr(1);
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##amount", &amount);
    ImGui::Button("Buy-in", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 4, 40));
    ImGui::SameLine();
    ImGui::Button("Cash-out", ImVec2(-1, 40));
    ImGui::EndDisabled();
    ImGui::TextDisabled("Coming with REQ-9 and REQ-14.");
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(0, ImGui::GetContentRegionAvail().y - 150));

    ImGui::BeginChild("settle", ImVec2(0, 0), ImGuiChildFlags_Borders);
    ImGui::Text("Buy-ins      %s", formatCents(s.potCents()).c_str());
    Cents cashOuts = 0;
    for (const auto& p : s.players()) if (p.cashOutCents) cashOuts += *p.cashOutCents;
    ImGui::Text("Cash-outs    %s", formatCents(cashOuts).c_str());
    ImGui::Text("Unaccounted  %s", formatCents(s.potCents() - cashOuts).c_str());
    ImGui::BeginDisabled();
    ImGui::Button("Settle up", ImVec2(-1, 44));
    ImGui::EndDisabled();
    ImGui::TextDisabled("Enabled when unaccounted = $0.00 (REQ-16).");
    ImGui::EndChild();
}

}  // namespace pokertab
