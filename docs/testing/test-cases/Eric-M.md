# Test Cases: Eric M

**Team number:** EM
**My area:** Backend/DB

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

**Test ID:** EM-01
**From Part 6:** (old test number, or New)
**Written by:** ERIC
**Title:** Backend writes to a file
**Traces to:** core/src/session_store.cpp
**Test type:** System
**Preconditions:** nil
**Test data:** nil

**Steps:**
1. Run the application
2. Save the work

**Expected result:** A file is created
**Actual result:** A file is indeed created
**Status:** Pass

---

**Test ID:** EM-02
**From Part 6:** (old test number, or New)
**Written by:** Eric
**Title:** The file includes data from the previous session
**Traces to:** core/src/session_store.cpp
**Test type:** System
**Preconditions:** a file was already saved
**Test data:** the already saved file

**Steps:**
1. Run the application
2. Attempt to load a previous save

**Expected result:** The previous save is loaded
**Actual result:** The previous save is loaded
**Status:** PAsS

---

**Test ID:** EM-03
**From Part 6:** (old test number, or New)
**Written by:** Eric
**Title:** New sessions with identical names are handled gracefully
**Traces to:** session_store.cpp
**Test type:** System
**Preconditions:** An existing session save
**Test data:**

**Steps:**
1. Create a session with an identical name as an existing session

**Expected result:** The system throws an error saying a session with that name already exists
**Actual result:** The old session is overwritten silently
**Status:** fail
**Severity:** Major
**V or V:** Verification
**GitHub Issue:** #101

---

**Test ID:** EM-03
**From Part 6:** (old test number, or New)
**Written by:** Eric Miao
**Title:** Only .pokertab.json files are shown
**Traces to:** Dashboard.cpp
**Test type:** Integration
**Preconditions:** Random other .json files exist in pokertab/sessions directory
**Test data:**

**Steps:**
1. Create random non pokertab json files in pokertab/sessions like pokertab/sessions/fjdskahf.json
2. See if they are listed on the New Session/load session screen

**Expected result:** Only .pokertab.json files are shown
**Actual result:** all .json files are shown
**Status:** fail
**Severity:** Minor
**V or V:** Validation
**GitHub Issue:** #102

---
