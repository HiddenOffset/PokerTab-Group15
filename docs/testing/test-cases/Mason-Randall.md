# Test Cases: Mason Randall

**Team number:** 01
**My area:** Frontend

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

**Test ID:** TC-MR-01
**From Part 6:** (old test number, or New) New
**Written by:** Mason Randall
**Title:** Creating a session with an empty name of user and rejcting empty player name  
**Traces to:** REQ-MR-02
**Test type:** (Unit / Integration / System / Acceptance / Boundary Value / Negative / Security) Negative
**Preconditions:** It should not run and block the input stating "Input a user name"
**Test data:**
- Player name: empty string 

**Steps:**
1. Open/create a session
2. find add player button and add wihtout a name
3. Click add player

**Expected result:** The player is not added, displays "Enter Player Name"
**Actual result:** States to enter a players name otherwise it will not appear empty 
**Status:** (Pass / Fail) Pass
**Severity:** (Critical / Major / Minor, Fail only) N/A
**V or V:** (Verification / Validation, Fail only) N/A
**GitHub Issue:** (#number, Fail only) N/A

---

**Test ID:** TC-MR-02
**From Part 6:** (old test number, or New) New
**Written by:** Mason Randall
**Title:** Adding a negative $ amount in the money value
**Traces to:** REQ-MR-01
**Test type:** Boundary Value 
**Preconditions:** Testing any negative dollar value -1/ 0/ and 1 $  
**Test data:**
- dollar amount: -1 Reject 
- dollar amount: 0 Reject 
- dollar amount: 1 Valid

**Steps:**
1. Open/create a session
2. Enter a value that is negative dollar value 1
3. Enter a value 0 dollar value
4. Enter a value positive dollar value 1

**Expected result:** Negative dollar value should display that you need to input a positive dollar value with at most 2 decimals. Then an input of 0 should say the same thing. Inputting a dollar value of 1 or above displays the amount with no error 
**Actual result:** Negative dollar value displays the prompt above as well as putting zero and putting 1 dollar value inputs it into the session
**Status:** Pass
**Severity:** N/A
**V or V:** N/A
**GitHub Issue:**

---

**Test ID:** TC-MR-03
**From Part 6:** (old test number, or New) New
**Written by:** Mason Randall
**Title:** The negative dollar amount should be red and the postive should be green
**Traces to:** NFR-MR-01
**Test type:** System
**Preconditions:** the session is open and the player has a buy-in amount and then a cash out amount
**Test data:**
-Buy in amount: 20.00
-Cash out amount: 10.00

**Steps:**
1. Open/Create new session
2. Input postive dollar amount
3. Take out dollar amount
4. Should display net in a color green or red depenidng on the net value

**Expected result:** Display green if the number is postive and red if it is negative net value 
**Actual result:** The app does not display the negative values in red or the postive in green
**Status:** Fail
**Severity:** Minor
**V or V:** Verification 
**GitHub Issue:** #76

---

**Test ID:** TC-MR-04
**From Part 6:** (old test number, or New) New
**Written by:** Mason Randall 
**Title:** Creating a valid session to host other poker users
**Traces to:** G-MR-01
**Test type:** Acceptance
**Preconditions:** Having the app open and is on the create new session area
**Test data:**
- Session name: "Poker Night Test #1"
- Buy in amount: 20.00

**Steps:**
1. Read the readme
2. Create a session with a valid name and dollar amount
3. Record Buy-in with a postive dollar amount
4. press enter to create the sesion
5. The balance should show along with the session name the amount of money in the pot and the players

**Expected result:** Display session with the valid name and dollar amount
**Actual result:** Display the session with the valid name and dollar amount
**Status:** pass
**Severity:** N/A
**V or V:** N/A
**GitHub Issue:**

---
