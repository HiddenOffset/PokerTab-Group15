# Test Cases: Seyar Najeed

**Team number:** 1
**My area:** Player Management

\<!-- HOW TO USE THIS FILE 1. Copy this file. Name the copy with your first name and last initial, like maria-g.md 2. Leave TEMPLATE.md untouched for your teammates. 3. If a Part 6 test belongs to your area, upgrade it first and put it in the first block.    Write at least 4 test cases using at least 3 test types. Include 1 Acceptance test that traces to your Stakeholder Goal.    Run every test on the inherited system. Every Fail gets a GitHub Issue. 4. Copy the block between the lines for each new test case. 5. Test IDs: use your initials plus a number, like MG-01, MG-02. This keeps IDs unique across your team.    "Traces to" must name one of YOUR requirement IDs, like REQ-MG-02. 6. See docs/TESTING_FIELD_GUIDE.md for the sample test case TC-00. -->

---

**Test ID:** SN-01
**From Part 6:** TC-2
**Written by:** Seyar N
**Title:** Removing a player who has no buy-in transactions
**Traces to:** REQ-SN-02
**Test type:** Unit, Positive
**Preconditions:** A session has been created and a player named Test has been added without any buy-ins.
**Test data:**

`Player name: Test`

**Steps:**

1. Create a new session.
2. Add a player named Test.
3. Try removing the player from the session.

**Expected result:** The player is removed from the session and no longer appears in the player list.
**Actual result:** The player was removed from the roster and the application displayed "Removed Test from the roster."
**Status:** Pass
**Severity:** Minor
**V or V:** Verification
**GitHub Issue:** None

---

**Test ID:** SN-02
**From Part 6:** TC-1
**Written by:** Seyar N
**Title:** Trying to remove a player who has already made a buy-in
**Traces to:** REQ-SN-02
**Test type:** Unit, Negative
**Preconditions:** A session has been created and a player named Test has made a buy-in.
**Test data:**

`Player name: Test`
`Buy-in: $20.00`

**Steps:**

1. Create a new session.
2. Add a player named Test.
3. Give the player a $20.00 buy-in.
4. Try removing the player.

**Expected result:** The player cannot be removed because they have a recorded transaction.
**Actual result:** The application did not allow the player to be removed after the $20.00 buy-in was recorded.
**Status:** Pass
**Severity:** Major
**V or V:** Verification
**GitHub Issue:** None

---

**Test ID:** SN-03
**From Part 6:** New
**Written by:** Seyar N
**Title:** Checking the information shown for each player
**Traces to:** REQ-SN-03
**Test type:** Integration
**Preconditions:** A poker session has been created and has at least one player.
**Test data:**

`Player name: Test`
`Buy-in: $20.00`

**Steps:**

1. Open a poker session.
2. Add a player named Test.
3. Give the player a $20.00 buy-in.
4. Look at the player roster.
5. Check the information shown next to the player's name.

**Expected result:** The roster displays the player's name, buy-in count, re-buy count, total amount paid in, cash-out amount, and net amount.
**Actual result:** The roster displayed the player's name, 1 buy-in, 0 re-buys, $20.00 total in, a dash under cash-out, and $0.00 net.
**Status:** Pass
**Severity:** Minor
**V or V:** Validation
**GitHub Issue:** None

---

**Test ID:** SN-04
**From Part 6:** New
**Written by:** Seyar N
**Title:** Adding and removing a player through the application
**Traces to:** G-SN-01
**Test type:** Acceptance
**Preconditions:** The PokerTab application is open.
**Test data:**

`Player name: test`

**Steps:**

1. Open or create a poker session.
2. Add a player named test.
3. Check that the player appears in the roster.
4. Select the Remove option for that player.
5. Check the roster again.

**Expected result:** The player is removed from the roster and the application displays a message confirming the removal.
**Actual result:** The player was removed from the roster, the player count changed to 0, and the application displayed "Removed test from the roster."
**Status:** Pass
**Severity:** Major
**V or V:** Validation
**GitHub Issue:** None

---

