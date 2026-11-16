# Traceability Matrix

**Team number:** 1
**Filled in by (captain):** Russell

Fill this in **after** everyone's work is merged. One row per requirement.

## Requirements to Tests

| Requirement ID | From Part 5 | Owner | Test case IDs | Result | GitHub Issue # |
|---|---|---|---|---|---|
| REQ-MG-01 *(example)* | CR-3 | Maria | MG-01, MG-02 | 1 Pass, 1 Fail | #12 |
| REQ-RN-01 | CR-5 | Russell | RN-04 | 1 Fail | #79 |
| REQ-RN-02 | New | Russell | None | N/A | N/A |
| REQ-RN-03 | CR-1 | Russell | RN-04 | 1 Fail | #78 |
| NFR-RN-01 | CR-5 | Russell | RN-01, RN-03 | 2 Fail | #63, #79 |
| REQ-MR-01 | CR-2 | Mason | TC-MR-02 | 1 Pass | N/A |
| REQ-MR-02 | CR-4 | Mason | TC-MR-01 | 1 Pass | N/A |
| REQ-MR-03 | New | Mason | None | N/A | N/A |
| NFR-MR-01 | New | Mason | TC-MR-03 | 1 Fail | #76 |
| REQ-SN-01 | CR-4 | Seyar | None | N/A | N/A |
| REQ-SN-02 | CR-3 | Seyar | SN-01, SN-02 | 2 pass | N/A |
| REQ-SN-03 | New | Seyar | SN-03 | 1 Pass | N/A |
| NFR-SN-01| New | Seyar | None | N/A | N/A |
| | | | | | |
| | | | | | |
| | | | | | |

## Gaps

List any requirement with no test, any test with no requirement, and any Part 5 requirement or Part 6 test that nobody brought over. Write "None" if there are none.

- REQ-RN-02
- REQ-MR-03
- CR-3

## Test Type Coverage

Put a Test ID next to each type. Team minimum: all 7.

| Test type | Test ID |
|---|---|
| Unit | RN-03 |
| Integration | RN-01 |
| System (E2E) | TC-MR-03 |
| Acceptance | RN-02 |
| Boundary Value | TC-MR-02 |
| Negative | TC-MR-04 |
| Security | RN-03 |

## Validation Findings

A Validation finding means the system does what the requirement says, but the requirement does not serve the Stakeholder Goal. Remember the alphabetical leaderboard.

| Requirement ID | Goal it fails | Why | Issue # |
|---|---|---|---|
| | | | |

**If you found none,** explain in 2 to 3 sentences why every requirement truly serves its Goal:

> The requirements supported stakeholder G-MR-01 by allowing the poker session creator to create a session as well as manage the players inside and the balances of said players as stated by REQ-MR-01 in the requirements.md. The Transactions requirements support G-RN-01 by being able to record players' transactions with money, as stated in REQ-RN-01. This would prevent invalid buy-ins and show transaction errors correctly; this was also stated in REQ-RN-03 and NFR-RN-01 in the requirements.md.

## Summary

| | Count |
|---|---|
| Part 5 requirements carried over | 4 |
| New requirements added | 2 |
| Part 6 tests upgraded (out of 3) | 3 |
| Total requirements | 6 |
| Total test cases | 8 |
| Passed | 3 |
| Failed | 5 |
| Issues opened | 5 |
| Critical issues | 3 |

**In one sentence, would you ship this system today? Why or why not?**

> I would not ship this system today as it lacks core functionality, especially surrounding with the requirements that the Accountant has when it comes to ensuring that when a player leaves, their cash-out is accurately recorded and their net winnings/losses are accurately computed.
