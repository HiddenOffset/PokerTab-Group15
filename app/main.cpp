// ============================================================================
// main.cpp — Poker Tab desktop dashboard (Dear ImGui + GLFW + OpenGL 3)
// ----------------------------------------------------------------------------
// Implements the wireframe's first two screens:
//   * New Session dialog        REQ-1 (create), REQ-3 (open recent)
//   * Dashboard                 REQ-5/6 (add player), REQ-24/25/26 (layout)
// Every change is written to the local session file at once (REQ-2/REQ-4).
// Buy-in, cash-out, undo and settle are drawn disabled; they arrive with
// REQ-9, REQ-14, REQ-22 and REQ-17.
// ============================================================================
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "pokertab/money.hpp"
#include "pokertab/session.hpp"
#include "pokertab/session_store.hpp"

namespace fs = std::filesystem;
using namespace pokertab;

namespace {

// ---- application state -----------------------------------------------------

struct App {
    std::optional<Session> session;
    fs::path sessionPath;

    // new-session dialog
    std::string newName;
    std::string newDefaultBuyIn = "20.00";
    std::string newError;
    std::vector<fs::path> recent;

    // dashboard
    std::string playerInput;
    std::string playerError;
    std::string status;      // confirmation strip (REQ-21)
    double statusUntil = 0;  // glfwGetTime() when the strip clears
    bool focusPlayerInput = false;
};

void showStatus(App& app, std::string text) {
    app.status = std::move(text);
    app.statusUntil = glfwGetTime() + 3.0;
}

void persist(App& app) {
    try {
        SessionStore::save(*app.session, app.sessionPath);
    } catch (const std::exception& e) {
        showStatus(app, std::string("Save failed: ") + e.what());
    }
}

std::vector<fs::path> listRecentSessions() {
    std::vector<fs::path> out;
    const fs::path dir = SessionStore::defaultSessionsDir();
    std::error_code ec;
    for (const auto& entry : fs::directory_iterator(dir, ec)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") out.push_back(entry.path());
    }
    std::sort(out.begin(), out.end(), [](const fs::path& a, const fs::path& b) {
        return a.filename().string() > b.filename().string();  // date prefix => newest first
    });
    if (out.size() > 8) out.resize(8);
    return out;
}

// ---- New Session dialog (REQ-1, REQ-3) ------------------------------------

void drawNewSessionDialog(App& app) {
    const ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(560, 0));
    if (!ImGui::IsPopupOpen("New session")) ImGui::OpenPopup("New session");

    if (!ImGui::BeginPopupModal("New session", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) return;

    ImGui::TextUnformatted("Session name");
    ImGui::SetNextItemWidth(-1);
    if (ImGui::IsWindowAppearing()) ImGui::SetKeyboardFocusHere();
    bool submit = ImGui::InputTextWithHint("##name", "Friday night", &app.newName, ImGuiInputTextFlags_EnterReturnsTrue);
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
    ImGui::InputText("##buyin", &app.newDefaultBuyIn);
    ImGui::TextDisabled("Pre-fills every buy-in.");
    ImGui::Columns(1);
    ImGui::Spacing();

    if (!app.recent.empty()) {
        ImGui::SeparatorText("Or open a recent session");
        for (const auto& p : app.recent) {
            ImGui::TextUnformatted(p.filename().string().c_str());
            ImGui::SameLine(ImGui::GetWindowWidth() - 70);
            ImGui::PushID(p.string().c_str());
            if (ImGui::SmallButton("Open")) {
                try {
                    app.session = SessionStore::load(p);
                    app.sessionPath = p;
                    app.newError.clear();
                    showStatus(app, "Opened " + p.filename().string());
                    ImGui::CloseCurrentPopup();
                } catch (const std::exception& e) {
                    app.newError = e.what();
                }
            }
            ImGui::PopID();
        }
        ImGui::Spacing();
    }

    if (!app.newError.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.25f, 0.25f, 1));
        ImGui::TextWrapped("%s", app.newError.c_str());
        ImGui::PopStyleColor();
    }

    ImGui::Separator();
    const float w = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + w - 160);
    if (ImGui::Button("Create session", ImVec2(160, 36)) || submit) {
        const std::string name = trimmed(app.newName);
        const auto buyIn = parseCents(app.newDefaultBuyIn);
        if (name.empty()) {
            app.newError = "Enter a session name.";
        } else if (!buyIn || *buyIn <= 0) {
            app.newError = "Default buy-in must be a positive dollar amount, e.g. 20.00";
        } else {
            Session s = Session::create(name);  // REQ-1: today's date
            s.setDefaultBuyInCents(*buyIn);
            app.sessionPath = SessionStore::pathFor(s);
            app.session = std::move(s);
            app.newError.clear();
            persist(app);
            showStatus(app, "Created session \"" + name + "\"");
            app.focusPlayerInput = true;
            ImGui::CloseCurrentPopup();
        }
    }
    ImGui::EndPopup();
}

// ---- Dashboard (REQ-5, REQ-24, REQ-25, REQ-26) -----------------------------

void addPlayer(App& app) {
    Session& s = *app.session;
    const std::string name = trimmed(app.playerInput);
    if (auto err = s.addPlayer(name)) {
        app.playerError = *err == AddPlayerError::EmptyName ? "Enter a player name."
                                                           : "\"" + name + "\" is already on the roster.";
        return;
    }
    app.playerError.clear();
    app.playerInput.clear();
    persist(app);
    showStatus(app, "Added " + name + " to the roster");
    app.focusPlayerInput = true;
}

void drawHeader(App& app) {
    const Session& s = *app.session;
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

void drawRoster(App& app) {
    const Session& s = *app.session;
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
    if (app.focusPlayerInput) {
        ImGui::SetKeyboardFocusHere();
        app.focusPlayerInput = false;
    }
    const bool enter = ImGui::InputTextWithHint("##player", "Name", &app.playerInput, ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    if (ImGui::Button("Add Player", ImVec2(120, 0)) || enter) addPlayer(app);
    if (!app.playerError.empty()) {
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.25f, 0.25f, 1));
        ImGui::TextUnformatted(app.playerError.c_str());
        ImGui::PopStyleColor();
    }

    // REQ-21 confirmation strip + undo placeholder
    ImGui::Spacing();
    ImGui::BeginChild("status", ImVec2(0, 40), ImGuiChildFlags_Borders);
    if (glfwGetTime() < app.statusUntil) ImGui::TextUnformatted(app.status.c_str());
    else ImGui::TextDisabled("Confirmations appear here for 3 seconds.");
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    ImGui::BeginDisabled();
    ImGui::Button("Undo last");
    ImGui::EndDisabled();
    ImGui::EndChild();
}

void drawSidePanel(App& app) {
    const Session& s = *app.session;
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

void drawDashboard(App& app) {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Session")) {
            if (ImGui::MenuItem("New session...")) {
                app.session.reset();
                app.newName.clear();
                app.recent = listRecentSessions();
            }
            ImGui::Separator();
            ImGui::TextDisabled("%s", app.sessionPath.string().c_str());
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    drawHeader(app);
    ImGui::Spacing();

    const float side = 300;
    ImGui::BeginChild("left", ImVec2(ImGui::GetContentRegionAvail().x - side - 12, 0));
    drawRoster(app);
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("right", ImVec2(side, 0));
    drawSidePanel(app);
    ImGui::EndChild();
}

}  // namespace

// ---- entry -----------------------------------------------------------------

int main() {
    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(1280, 800, "Poker Tab", nullptr, nullptr);
    if (!window) { glfwTerminate(); return 1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;  // no imgui.ini next to the exe
    ImGui::StyleColorsLight();
    ImGui::GetStyle().FrameRounding = 3;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    App app;
    app.recent = listRecentSessions();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const ImGuiViewport* vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(vp->WorkPos);
        ImGui::SetNextWindowSize(vp->WorkSize);
        ImGui::Begin("Poker Tab", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
        if (app.session) drawDashboard(app);
        else drawNewSessionDialog(app);
        ImGui::End();

        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.96f, 0.96f, 0.95f, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
