# Poker Tab

Poker Tab is a Windows desktop app that keeps the money straight at a casual
poker night. One person, the accountant, records every buy-in, re-buy and
cash-out; at the end Poker Tab computes each player's net and the fewest
payments that settle everyone up.

Requirements: [Requirement Analysis Document](docs/RAD.pdf) · Build and
contribute: [docs/DEVELOPMENT.md](docs/DEVELOPMENT.md)

## Status

| Requirement | State |
| --- | --- |
| REQ-1 Create a session | Done — name, today's date, saved to disk |
| REQ-2 / REQ-3 Save and load | Done — one JSON file per session |
| REQ-5 / REQ-6 Add player, reject duplicates | Done |
| REQ-9 Buy-ins and re-buys | Next |
| REQ-14 Cash-outs | Next |
| REQ-17 to REQ-19 Settlement | Planned |

Wireframes: [docs/wireframes](docs/wireframes) · Screenshots: [docs/screenshots](docs/screenshots)

## Quick start

```
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Then run `build/app/pokertab` (or `build\app\Debug\pokertab.exe` on Windows).

## Team

| Member | Role |
| --- | --- |
| Travis Trinidad | Narrative Lead |
| Nishanth Mahendran | Front-End |
| Matthew Rohrer | Back-End |
| David Martindale | Problem Framer |

CS course project, SDSU, Fall 2026.
