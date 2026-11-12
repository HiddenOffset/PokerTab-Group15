# Test Cases: Russell N

**Team number:** 1
**My area:** Transactions

<!--
HOW TO USE THIS FILE
1. Copy this file. Name the copy with your first name and last initial, like  maria-g.md
2. Leave TEMPLATE.md untouched for your teammates.
3. If a Part 6 test belongs to your area, upgrade it first and put it in the first block.
   Write at least 4 test cases using at least 3 test types. Include 1 Acceptance test that traces to your Stakeholder Goal.
   Run every test on the inherited system. Every Fail gets a GitHub Issue.
4. Copy the block between the lines for each new test case.
5. Test IDs: use your initials plus a number, like  MG-01, MG-02. This keeps IDs unique across your team.
   "Traces to" must name one of YOUR requirement IDs, like REQ-MG-02.
6. See docs/TESTING_FIELD_GUIDE.md for the sample test case TC-00.
-->

---

**Test ID:** RN-01
**From Part 6:** TC-3
**Written by:** Russell N
**Title:** 100 Trillion in single buy-in gets rejected
**Traces to:** NFR-RN-01
**Test type:** Integration, Negative, Boundary
**Preconditions:** A session has been created and a player with `player-id = 1` to assign the buy-in to already exists within the session.
**Test data:**

`onBuyIn(1, "99999999999")`
`onBuyIn(1, "100000000000")`
`onBuyIn(1, "100000000001")`

**Steps:**
1. In a session with an existing user with playerId = 1, enter in 999999999.99 in BUY-IN's Amount input field.
2. In a session with an existing user with playerId = 1, enter in 1000000000.00 in BUY-IN's Amount input field.
3. In a session with an existing user with playerId = 1, enter in 1000000000.01 in BUY-IN's Amount input field.

**Expected result:** "Buy-in is too large for this session."
**Actual result:** "Enter a positive dollar amount with at most two decimal places."
**Status:** Fail
**Severity:** Minor
**V or V:** Verification
**GitHub Issue:** #63

---

**Test ID:** RN-02 
**From Part 6:** REQ-RN-01
**Written by:** Russell N
**Title:** A player can join and leave a session with their exact winnings and losses accounted for.
**Traces to:** G-RN-01
**Test type:** Acceptance
**Preconditions:** A session has already been created.
**Test data:**

`playerName: Test`
`Amount (Buy-in): 20.00`
`Amount (Cash-out): 20.00`

**Steps:**
1. Add a player using the test data into the session.
2. Record the buy-in of the player added using the test data through the "BUY-IN" window.
3. Record the cash-out of the player using the test data through the "BUY-IN" window.

**Expected result:** Roster window records: "Test, 0, 0 , $20.00, $20.00, $0.00, Buy-in (greyed out), Cash-out (greyed out), Remove".
**Actual result:** Roster window records: "Test, 0, 0, $20.00, -, $0.00, Buy-in, Cash-out (greyed out), Remove"
**Status:** Fail
**Severity:** Major
**V or V:** Validation
**GitHub Issue:** 80

---

**Test ID:** RN-03
**From Part 6:** New
**Written by:** Russell N
**Title:** recordCashOut() disables functionality after the first cash-out.
**Traces to:** NFR-RN-01
**Test type:** Unit, Security
**Preconditions:** A session exists with a player that has playerId = 1 and that player has a buy-in worth $20.00.
**Test data:**

`recordCashOut(1, 1)`
`recordCashOut(1, 1)`

**Steps:**
1. Call recordCashOut(1, 1) for the first time.
2. Call recordCashOut(1, 1) for the second time.

**Expected result:** recordCashOut returns an error.
**Actual result:** Nothing happens as recordCashOut is not implemented.
**Status:** Fail
**Severity:** Major
**V or V:** Verification
**GitHub Issue:**

---

**Test ID:** RN-04
**From Part 6:** REQ-RN-01
**Written by:** Russell N
**Title:** recordCashOut() records into a player's cashOutCents
**Traces to:** REQ-RN-01
**Test type:** Integration
**Preconditions:** A session exists with Player player that has playerId = 1 and that player has a buy-in worth $20.00.
**Test data:** #79

`recordCashOut(1, 1)`

**Steps:**
1. Call recordCashOut(1, 1)

**Expected result:** findPlayer(1)->cashOutCents == 1
**Actual result:** findPlayer(1)->cashOutCents = 0
**Status:** Fail
**Severity:** Major
**V or V:** Verification
**GitHub Issue:** #78

---
