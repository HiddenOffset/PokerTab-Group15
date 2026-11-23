# PR4 Team 01 Report

**Team Number:** 01
**Team Members:** Russell Nguyen, Eric, Mason Randall, Seyar Najeed

## 1. Bug Triage Table

|Test name| Bucket | Issue link | Severity | Priority | Owner | Status |
|Duplicate session names| Code bug |https://github.com/HiddenOffset/PokerTab-Group15/issues/95| Medium | Medium | Seyar | Fixed |
|Remove button not working| Code bug |https://github.com/HiddenOffset/PokerTab-Group15/issues/100| Medium | Medium | Seyar | Fixed |
|100 Trillion in single buy-in gets rejected with AmountTooLarge error | Code | https://github.com/HiddenOffset/PokerTab-Group15/issues/63 | Low | Low | Russell | Fixed
Cash-out button records how much a player has when they leave a session and marks them as cashed out | Code | https://github.com/HiddenOffset/PokerTab-Group15/issues/94 | High | High | Russell | Fixed

## 2. Regression Log



## 3. Final Test Run



## 4. Feature Design 

**Chosen feature":** Settle Up

**User Story:** as the Accountant, I want to easily see who can pay their losses to a person who earned their winings, so that everyone is content with the results of the session after it concludes.

**Why we chose it:** We chose it as this is a feature that is decently complex, allowing us to divide its parts for our group of 4 to contribute individual parts, as well as something that was already considered by the original group with a framework that we can easily follow as if we were the ones who came up with it ourselves.

**Ownership:** 

-[Owner Name:Russell] (work in session.cpp and how settle-up works behind the scenes)

-[Owner Name:Eric] (work on how settle-up is stored and tracked in the JSON)

-[Owner Name:Mason] (work on how the settle-up button is displayed and with what values it displays certain text)

-[Owner Name:Seyar] (work on wiring dashboard.cpp to session.cpp)


**Two test cases:**
1. One test case will show that settle-up will only run when everyone has cashed out. This ensures that there is no one left playing and that the session is truly over before moving money around for the final time.
1. Another test case will be that settle-up accurately describes those who have a net loss will pay those who have a net gain. This is so everyone pays the right amount of money to the right people, ensuring losers pay exactly what they lost and winners get paid exactly what they have won.


## 5. Board Snapshots


## 6. Sprint Summary
