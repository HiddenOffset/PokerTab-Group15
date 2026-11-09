# Testable Requirements: PokerTab

**Team number:** 1
**Inherited from:** Team 15

## Who Owns What

| Area of the system | Owner | ID prefix |
|---|---|---|
| Transactions | Russell Nguyen | RN |
| Frontend | Mason Randall | MR |
| | | |
| | | |

<!--
RULES
- Only edit YOUR OWN section below. This keeps merge conflicts away.
- Categories: G = Stakeholder Goal, REQ = Functional, NFR = Non-Functional.
- "From Part 5": write the original CR ID and its text from SYSTEM_MAP.md. Write "New" if you added it this sprint.
- Status: Confirmed or Suspected bug. No Unconfirmed allowed. Every Suspected bug needs a GitHub Issue.
- The test: could a stranger write a Pass or Fail test without asking you a question?
-->

---

## Section: Transactions (Russell Nguyen)

### Stakeholder Goals

| ID | Stakeholder | Goal |
|---|---|---|
| G-RN-01 | Accountant | Keep accurate transactions of players that have both entered and exited the games so that everyone gets pays what they lose or gets paid out what they win. |

### Functional Requirements

| ID | Requirement | Status | From Part 5 |
|---|---|---|---|
| REQ-RN-01 | The system shall let a player cash-out. | Confirmed | CR-5 |
| REQ-RN-02 | The system shall let the most recent transaction be reversible. | Confirmed | New |
| REQ-RN-03 | The system shall reject buy-ins of $100,000,000,000.00+. | Suspected Bug | CR-1 |

### Non-Functional Requirements

| ID | Requirement | Status | From Part 5 |
|---|---|---|---|
| NFR-RN-01 | The system shall let the accountant know in clear terms why a checkout failed using error enums, such as Player Not Found, Invalid Amount, or Amount Too Large. | Usability | CR-5 |

### Example of a Good Rewrite

| ID | Requirement | Status | From Part 5 |
|---|---|---|---|
| REQ-MG-01 | The system shall reject a profile bio longer than 280 characters with the error "Bio must be 280 characters or fewer." | Suspected bug | CR-3 (Unconfirmed): The system shall limit the bio to 280 characters. |
| NFR-MG-01 | The profile page loads in under 2 seconds with 100 profiles in the database. | Confirmed | New |

---

## Section: [Your Area] ([Your Name])

### Stakeholder Goals

| ID | Stakeholder | Goal |
|---|---|---|
| | | |

### Functional Requirements

| ID | Requirement | Status | From Part 5 |
|---|---|---|---|
| | | | |
| | | | |
| | | | |

### Non-Functional Requirements

| ID | Requirement | Status | From Part 5 |
|---|---|---|---|
| | | | |

---

## Section: [Your Area] ([Your Name])

### Stakeholder Goals

| ID | Stakeholder | Goal |
|---|---|---|
| | | |

### Functional Requirements

| ID | Requirement | Status | From Part 5 |
|---|---|---|---|
| | | | |
| | | | |
| | | | |

### Non-Functional Requirements

| ID | Requirement | Status | From Part 5 |
|---|---|---|---|
| | | | |

---

## Section: [Frontend] ([Mason Randall])

### Stakeholder Goals

| ID | Stakeholder | Goal |
|---|---|---|
| G-MR-01 | Poker session creator | Will manage the session and players inside the session while displaying the balance of players on the dashboard |

### Functional Requirements

| ID | Requirement | Status | From Part 5 |
|---|---|---|---|
| REQ-MR-01| The system shall create a poker session when the user submits a session name and valid buy-in amount | Confirmed | CR-2 |
| REQ-MR-02| The system shall allow for players with a non-empty name to be added and implemented in a session | Confirmed | CR-4 |
| REQ-MR-03| The system shall compute and display each player's net as cash-out minus total buy-ins | Confirmed | new |

### Non-Functional Requirements

| ID | Requirement | Status | From Part 5 |
|---|---|---|---|
| NFR-MR-01 | The system shall display a dashboard and show positive net amounts in green and negative net amounts in red when losing money without needing to calculate manually | Suspected bug | new |
