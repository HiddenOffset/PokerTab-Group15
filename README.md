# Poker Tab

Poker Tab is a Windows desktop app that keeps the money straight at a casual
poker night. One person, the accountant, records every buy-in, re-buy and
cash-out; at the end Poker Tab computes each player's net and the fewest
payments that settle everyone up.

Requirements: [Requirement Analysis Document](docs/RAD.pdf) · Build and
contribute: [docs/DEVELOPMENT.md](docs/DEVELOPMENT.md)

## Status

PokerTab as of 10/14/2026, is ready to be handed off to the next group. 

## Features to fix/add to complete PokerTab
-
-
-


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
