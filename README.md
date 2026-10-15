# Poker Tab

Poker Tab is a desktop app that keeps the money straight at a casual poker
night. One person, the accountant, records every buy-in, re-buy and
cash-out as it happens; the app saves after each change and keeps a running
pot.

Requirements: [Requirement Analysis Document](docs/RAD.pdf) · Design:
[docs/uml](docs/uml) · Build and contribute:
[docs/DEVELOPMENT.md](docs/DEVELOPMENT.md)

## Status as of October 14, 2026

Builds on macOS, Linux and Windows. Thirteen tests pass in CI on every pull
request. Ready to hand off.

| Use case | REQ | State |
| --- | --- | --- |
| Create session | REQ-1, 2, 3, 4 | Done. Name and date, saved to one JSON file, autosave after every change, reopen from the New Session dialog. Tested. |
| Add player | REQ-5, 6, 8 | Done. Duplicate names refused, case-insensitive. Tested. |
| Remove player | REQ-7 | Done. No test yet. |
| Record buy-in | REQ-9, 10, 11 | Done. Re-buys accumulate, bad amounts refused, overflow guarded. Tested. |
| Record cash-out | REQ-14, 15 | Not done. The button is wired but the handler is a commented-out sketch; clicking it does nothing. |
| Undo last transaction | REQ-22 | Not done. Empty stub. |
| Settle up | REQ-16, 17, 18 | Dropped from scope on October 14 (PR #47). The RAD still describes it. |

## Features to fix or add to complete Poker Tab

In the order the next group should take them. Each one has a named method
in [docs/uml/naming-contract.md](docs/uml/naming-contract.md) and a stub in
`app/dashboard.cpp`.

1. **Record cash-out (REQ-14, REQ-15).** Add `Session::recordCashOut(playerId, amount)` with a `CashOutError` enum next to `BuyInError`, fill in `Dashboard::onCashOut`, disable Buy-in for a cashed-out player, and add tests. The Cash-out button already calls the handler.
2. **Fix `Player::totalOutCents()`.** It calls `cashOutCents.value()`, which throws for any player who has not cashed out. Nothing calls it yet, so it hasn't crashed, but the first caller will. Return `cashOutCents.value_or(0)`.
3. **Undo last transaction (REQ-22).** `Session::undoLast()` and `Dashboard::onUndo`. The Undo button is drawn disabled.
4. **Tests for remove player (REQ-7).** One `TEST_CASE` in `tests/test_session.cpp` covering remove, remove-nonexistent, and that the pot updates.
5. **Decide what happens to settlement.** The RAD (REQ-16 to 18, Appendix A), the proposal, and the merged UML all describe computing who pays whom. The team removed it from the code on October 14. Either put it back (the algorithm is in RAD Appendix A and `docs/uml/src/sequence-settle-up.puml`) or update the RAD so the documents agree with the product.
6. **Smaller items from the RAD.** Edit or delete a single transaction (REQ-23), export the session as text (REQ-19), color positive and negative nets (REQ-27), a confirmation strip timeout that doesn't depend on frame rate.

Wireframes: [docs/wireframes](docs/wireframes) · Screenshots: [docs/screenshots](docs/screenshots)

## Quick start

```
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Then run `build/app/pokertab` (macOS, Linux) or `build\app\Debug\pokertab.exe`
(Windows). Sessions are saved to `~/.pokertab/sessions/` or
`%APPDATA%\PokerTab\sessions\`.

## Team

| Member | Role |
| --- | --- |
| Travis Trinidad | Narrative Lead |
| Nishanth Mahendran | Front-End |
| Matthew Rohrer | Back-End |
| David Martindale | Problem Framer |

CS250, SDSU, Fall 2026.