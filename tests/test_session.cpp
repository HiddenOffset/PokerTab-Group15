// ============================================================================
// test_session.cpp — acceptance tests for REQ-1, REQ-2, REQ-3, REQ-5, REQ-6
// ----------------------------------------------------------------------------
// Each TEST_CASE is tagged with the REQ it checks so `ctest` / Catch2 output
// reads against the RAD. Run one REQ alone with:  pokertab_tests "[REQ-5]"
// ============================================================================
#include <catch_amalgamated.hpp>

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

#include "pokertab/money.hpp"
#include "pokertab/session.hpp"
#include "pokertab/session_store.hpp"

using namespace pokertab;

// ---- REQ-1: create session -------------------------------------------------

TEST_CASE("REQ-1: new session has the given name, today's date, no players, $0.00 pot", "[REQ-1]") {
    Session s = Session::create("Friday night");

    CHECK(s.name() == "Friday night");
    CHECK(s.date() == todayIsoDate());
    CHECK(s.date().size() == 10);          // YYYY-MM-DD
    CHECK(s.date()[4] == '-');
    CHECK(s.players().empty());
    CHECK(s.potCents() == 0);
    CHECK(formatCents(s.potCents()) == "$0.00");
}

TEST_CASE("REQ-1: session name is trimmed", "[REQ-1]") {
    CHECK(Session::create("  Tuesday  ").name() == "Tuesday");
}

// ---- REQ-5 / REQ-6: add player ---------------------------------------------

TEST_CASE("REQ-5: adding a player shows them on the roster with $0.00 in and $0.00 out", "[REQ-5]") {
    Session s = Session::create("Test", "2026-09-22");

    REQUIRE_FALSE(s.addPlayer("Alice").has_value());

    REQUIRE(s.players().size() == 1);
    const Player& alice = s.players().back();
    CHECK(alice.name == "Alice");
    CHECK(alice.id == 1);
    CHECK(alice.buyInCount() == 0);
    CHECK(alice.totalInCents() == 0);
    CHECK_FALSE(alice.cashedOut());
    CHECK(formatCents(alice.totalInCents()) == "$0.00");
    CHECK(formatCents(alice.netCents()) == "$0.00");
}

TEST_CASE("REQ-5: players get sequential ids and keep insertion order", "[REQ-5]") {
    Session s = Session::create("Test", "2026-09-22");
    s.addPlayer("Alice");
    s.addPlayer("Bob");
    s.addPlayer("Carol");

    REQUIRE(s.players().size() == 3);
    CHECK(s.players()[0].id == 1);
    CHECK(s.players()[1].id == 2);
    CHECK(s.players()[2].id == 3);
    CHECK(s.players()[2].name == "Carol");
    CHECK(s.findPlayer(2)->name == "Bob");
    CHECK(s.findPlayer(99) == nullptr);
}

TEST_CASE("REQ-5: names are trimmed; blank names are rejected", "[REQ-5]") {
    Session s = Session::create("Test", "2026-09-22");

    CHECK(s.addPlayer("  Dave ") == std::nullopt);
    CHECK(s.players().back().name == "Dave");

    CHECK(s.addPlayer("") == AddPlayerError::EmptyName);
    CHECK(s.addPlayer("   ") == AddPlayerError::EmptyName);
    CHECK(s.players().size() == 1);
}

TEST_CASE("REQ-6: duplicate player name is rejected and roster unchanged", "[REQ-6]") {
    Session s = Session::create("Test", "2026-09-22");
    s.addPlayer("Alice");

    CHECK(s.addPlayer("Alice") == AddPlayerError::DuplicateName);
    CHECK(s.addPlayer("alice") == AddPlayerError::DuplicateName);   // case-insensitive
    CHECK(s.addPlayer(" Alice ") == AddPlayerError::DuplicateName); // whitespace-insensitive
    CHECK(s.players().size() == 1);
}

TEST_CASE("REQ-8: at least 10 players fit in one session", "[REQ-8]") {
    Session s = Session::create("Test", "2026-09-22");
    for (int i = 1; i <= 10; ++i) REQUIRE(s.addPlayer("P" + std::to_string(i)) == std::nullopt);
    CHECK(s.players().size() == 10);
}

// ---- REQ-2 / REQ-3: save and load ------------------------------------------

namespace {
std::filesystem::path tempFile(const char* name) {
    return std::filesystem::temp_directory_path() / "pokertab_tests" / name;
}
}  // namespace

TEST_CASE("REQ-2/REQ-3: save then load restores every player exactly", "[REQ-2][REQ-3]") {
    Session s = Session::create("Round trip", "2026-09-22");
    s.setDefaultBuyInCents(2500);
    s.addPlayer("Alice");
    s.addPlayer("Bob");
    // simulate data later REQs will write, to prove the file carries it
    s.players()[0].transactions.push_back({1, "2026-09-22T20:00:00", 2000});
    s.players()[0].transactions.push_back({2, "2026-09-22T21:00:00", 2000});
    s.players()[1].cashOutCents = 3500;

    const auto path = tempFile("roundtrip.pokertab.json");
    SessionStore::save(s, path);
    REQUIRE(std::filesystem::exists(path));

    Session back = SessionStore::load(path);
    CHECK(back.name() == "Round trip");
    CHECK(back.date() == "2026-09-22");
    CHECK(back.defaultBuyInCents() == 2500);
    CHECK(back.nextPlayerId() == 3);
    REQUIRE(back.players().size() == 2);
    CHECK(back.players()[0].name == "Alice");
    CHECK(back.players()[0].id == 1);
    CHECK(back.players()[0].buyInCount() == 2);
    CHECK(back.players()[0].totalInCents() == 4000);
    CHECK(back.players()[0].transactions[1].timestamp == "2026-09-22T21:00:00");
    CHECK(back.players()[1].cashOutCents == 3500);
    CHECK(back.potCents() == s.potCents());

    // ids keep advancing after a reload, so no collision with saved players
    back.addPlayer("Carol");
    CHECK(back.players().back().id == 3);

    std::filesystem::remove(path);
}

TEST_CASE("REQ-3: loading a missing or corrupt file throws instead of returning garbage", "[REQ-3]") {
    CHECK_THROWS_AS(SessionStore::load(tempFile("does_not_exist.json")), std::runtime_error);

    const auto bad = tempFile("corrupt.json");
    std::filesystem::create_directories(bad.parent_path());
    std::ofstream(bad) << "{ this is not json";
    CHECK_THROWS_AS(SessionStore::load(bad), std::runtime_error);
    std::filesystem::remove(bad);

    CHECK_THROWS_AS(SessionStore::fromJson(R"({"schema": 99, "name": "x", "date": "y", "players": []})"),
                    std::runtime_error);
}

// ---- Money (NFR-5 / REQ-11 groundwork) ------------------------------------

TEST_CASE("money: parse and format whole cents", "[money][REQ-11]") {
    CHECK(parseCents("20") == 2000);
    CHECK(parseCents("20.5") == 2050);
    CHECK(parseCents("20.50") == 2050);
    CHECK(parseCents("$20.50") == 2050);
    CHECK(parseCents(" 0.05 ") == 5);

    CHECK_FALSE(parseCents("").has_value());
    CHECK_FALSE(parseCents("abc").has_value());
    CHECK_FALSE(parseCents("-5").has_value());
    CHECK_FALSE(parseCents("10.001").has_value());
    CHECK_FALSE(parseCents("1e3").has_value());

    CHECK(formatCents(0) == "$0.00");
    CHECK(formatCents(5) == "$0.05");
    CHECK(formatCents(2050) == "$20.50");
    CHECK(formatCents(-1500) == "-$15.00");
}
