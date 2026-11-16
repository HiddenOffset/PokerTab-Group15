# Test Cases: Seyar Najeed

**Team number:** 1
**My area:** Player Management

---

**Test ID:** SN-01
**From Part 6:** TC-2
**Written by:** Seyar N
**Title:** Removing a player with no buy-ins
**Traces to:** REQ-SN-02
**Test type:** Unit, Positive
**Preconditions:** A session is open and a player named Test has been added without making a buy-in.
**Test data:**

`Player name: Eric`

**Steps:**
1. Create a new session.
2. Add a player named Test.
3. Click Remove next to the player's name.

**Expected result:** The player should be removed from the roster.
**Actual result:** The player was removed, and the message "Removed Test from the roster." appeared.
**Status:** Pass

---

**Test ID:** SN-02
**From Part 6:** TC-1
**Written by:** Seyar N
**Title:** Trying to remove a player after a buy-in
**Traces to:** REQ-SN-02
**Test type:** Unit, Negative
**Preconditions:** A session is open and a player named Test has made a buy-in.
**Test data:**

`Player name: Seyar`
`Buy-in: $20.00`

**Steps:**
1. Create a new session.
2. Add a player named Test.
3. Record a $20.00 buy-in for the player.
4. Try to remove the player from the roster.

**Expected result:** The player should not be removed because they have a recorded transaction.
**Actual result:** The application did not allow the player to be removed after the buy-in.
**Status:** Pass

---

**Test ID:** SN-03
**From Part 6:** New
**Written by:** Seyar N
**Title:** Checking the player information in the roster
**Traces to:** REQ-SN-03
**Test type:** Integration
**Preconditions:** A session is open with at least one player who has made a buy-in.
**Test data:**

`Player name: Mason`
`Buy-in: $20.00`

**Steps:**
1. Open a poker session.
2. Add a player named Test.
3. Give the player a $20.00 buy-in.
4. Check the information shown in the roster.

**Expected result:** The roster should show the player's name, number of buy-ins and re-buys, total amount paid in, cash-out, and net amount.
**Actual result:** The roster showed Test, 1 buy-in, 0 re-buys, $20.00 total in, a dash for cash-out, and $0.00 net.
**Status:** Pass

---

**Test ID:** SN-04
**From Part 6:** New
**Written by:** Seyar N
**Title:** Adding a player and then removing them
**Traces to:** G-SN-01
**Test type:** Acceptance
**Preconditions:** The PokerTab application is open.
**Test data:**

`Player name: Russell`

**Steps:**
1. Open or create a session.
2. Add a player named test.
3. Make sure the player appears in the roster.
4. Click Remove next to the player's name.
5. Check the roster again.

**Expected result:** The player should disappear from the roster and a confirmation message should appear.
**Actual result:** The player disappeared from the roster, the player count went back to 0, and the application displayed "Removed test from the roster."
**Status:** Pass

---
