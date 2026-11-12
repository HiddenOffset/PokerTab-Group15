// Automated transaction cases from docs/testing/test-cases/RUSSELL-N.md.
// These assert the expected behavior, including the documented failures.
#include <catch_amalgamated.hpp>

#include <type_traits>
#include <utility>

#include "pokertab/session.hpp"

using namespace pokertab;

namespace {
    template <typename T, typename = void>
    struct HasRecordCashOut : std::false_type {};

    template <typename T>
    struct HasRecordCashOut<T, std::void_t<decltype(
        std::declval<T&>().recordCashOut(1, Cents{1}))>> : std::true_type {};

    // shortcut: detect the planned cents API so missing cash-out fails at runtime;
    // use direct calls once Session::recordCashOut exists.
    template <typename T>
    bool cashOutRejected(T& session, int playerId, Cents cents) {
        if constexpr (HasRecordCashOut<T>::value) {
            return session.recordCashOut(playerId, cents).has_value();
        } else {
            UNSCOPED_INFO("Session::recordCashOut(playerId, cents) is not implemented.");
            return true;
        }
    }
}  // namespace

TEST_CASE("RN-01: oversized buy-ins report AmountTooLarge and change nothing",
          "[RN][RN-01][NFR-RN-01][REQ-RN-03][integration][negative][boundary]") {
    // The document's Test data and Steps list different dollar amounts.
    // Use the explicit Test data strings and their stated expected error (#63).
    const auto amount = GENERATE("99999999999", "100000000000", "100000000001");
    CAPTURE(amount);
    Session s = Session::create("Test", "2026-11-12");
    REQUIRE_FALSE(s.addPlayer("Test").has_value());

    // Dashboard::onBuyIn maps this error to "Buy-in is too large for this session."
    CHECK(s.recordBuyIn(1, amount) == BuyInError::AmountTooLarge);

    const Player* player = s.findPlayer(1);
    REQUIRE(player != nullptr);
    CHECK(player->transactions.empty());
    CHECK(player->totalInCents() == 0);
    CHECK_FALSE(player->cashedOut());
    CHECK(s.potCents() == 0);
    CHECK(s.nextTransactionId() == 1);
}

TEST_CASE("RN-02: a player buys in and cashes out with exact totals",
          "[RN][RN-02][G-RN-01][REQ-RN-01][acceptance]") {
    Session s = Session::create("Test", "2026-11-12");
    REQUIRE_FALSE(s.addPlayer("Test").has_value());
    REQUIRE_FALSE(s.recordBuyIn(1, "20.00").has_value());
    REQUIRE_FALSE(cashOutRejected(s, 1, 2000));

    REQUIRE(s.players().size() == 1);
    const Player* player = s.findPlayer(1);
    REQUIRE(player != nullptr);
    CHECK(player->name == "Test");
    // Keep the recorded buy-in history; the source row's zero buy-ins conflicts
    // with its $20.00 total in. Button appearance remains a manual UI check.
    CHECK(player->buyInCount() == 1);
    CHECK(player->totalInCents() == 2000);
    CHECK(formatCents(player->totalInCents()) == "$20.00");
    REQUIRE(player->cashOutCents.has_value());
    CHECK(*player->cashOutCents == 2000);
    CHECK(formatCents(*player->cashOutCents) == "$20.00");
    CHECK(player->cashedOut());
    CHECK(player->netCents() == 0);
    CHECK(formatCents(player->netCents()) == "$0.00");

    // A cashed-out player cannot add another buy-in.
    CHECK(s.recordBuyIn(1, "20.00").has_value());
    CHECK(player->totalInCents() == 2000);
    CHECK(player->buyInCount() == 1);
}

TEST_CASE("RN-03: a second cash-out is rejected without changing the first",
          "[RN][RN-03][NFR-RN-01][unit][security]") {
    Session s = Session::create("Test", "2026-11-12");
    REQUIRE_FALSE(s.addPlayer("Test").has_value());
    REQUIRE_FALSE(s.recordBuyIn(1, "20.00").has_value());
    REQUIRE_FALSE(cashOutRejected(s, 1, 1));

    const Player* player = s.findPlayer(1);
    REQUIRE(player != nullptr);
    REQUIRE(player->cashOutCents == 1);
    const int nextTransactionId = s.nextTransactionId();

    CHECK(cashOutRejected(s, 1, 1));
    CHECK(player->cashOutCents == 1);
    CHECK(player->cashedOut());
    CHECK(player->totalInCents() == 2000);
    CHECK(player->buyInCount() == 1);
    CHECK(s.potCents() == 2000);
    CHECK(s.nextTransactionId() == nextTransactionId);
}

TEST_CASE("RN-04: a one-cent cash-out is stored in the player's cashOutCents",
          "[RN][RN-04][REQ-RN-01][integration]") {
    Session s = Session::create("Test", "2026-11-12");
    REQUIRE_FALSE(s.addPlayer("Test").has_value());
    REQUIRE_FALSE(s.recordBuyIn(1, "20.00").has_value());

    REQUIRE_FALSE(cashOutRejected(s, 1, 1));

    const Player* player = s.findPlayer(1);
    REQUIRE(player != nullptr);
    REQUIRE(player->cashOutCents.has_value());
    CHECK(*player->cashOutCents == 1);
    CHECK(player->cashedOut());
    CHECK(player->totalInCents() == 2000);
    CHECK(player->netCents() == -1999);
}
