# Naming Contract

The class, attribute, and method names every diagram and every source file
must use. Source of truth is the code (`core/include/pokertab/*.hpp`,
`app/dashboard.hpp`); this file records the names and marks what is
implemented versus planned. If a name changes in code, change it here and in
every diagram in the same pull request.

Convention: classes `PascalCase`, methods and attributes `camelCase`,
private members end in `_`, money is always `Cents` (int64, whole cents).
One class per file, named after the class.

## Shared classes and owners

Only the owner changes a class's file. Everyone else uses it. Add a row the
same day you need a class that is not here.

| Class | File | Owner | Used By |
| --- | --- | --- | --- |
| Session | `core/include/pokertab/session.hpp` | David | David, Travis, Nishanth, Matthew |
| Player | `core/include/pokertab/player.hpp` | David | David, Travis, Nishanth, Matthew |
| Transaction | `core/include/pokertab/transaction.hpp` | Matthew | Travis, Matthew |
| Money | `core/include/pokertab/money.hpp` | Matthew | David, Travis, Nishanth, Matthew |
| SessionStore | `core/include/pokertab/session_store.hpp` | David | David, Travis, Nishanth, Matthew |
| Settlement, Payment | `core/include/pokertab/settlement.hpp` (planned) | Matthew | Matthew |
| Dashboard | `app/dashboard.hpp` | Nishanth | David, Travis, Nishanth, Matthew |

Owners are proposed from the RAD roles; change them here if the team
decides otherwise.

## Actors

| Actor | Who |
| --- | --- |
| Accountant | The one person operating Poker Tab at the table (primary actor, every use case) |
| Player | Anyone at the table; reads the settlement screen, operates nothing (secondary actor, Settle Up only) |

## Use cases

Four slices and one shared use case. The shared one appears only in the
merged use case diagram; nobody draws a sequence diagram for it.

| Use case | Owner | Branch | REQ | Status |
| --- | --- | --- | --- | --- |
| Create Session | David Martindale | `david-create-session` | REQ-1, REQ-2 | Implemented |
| Add Player | TBD | | REQ-5, REQ-6 | Implemented |
| Record Buy-in | TBD | | REQ-9, REQ-10, REQ-11 | Planned |
| Record Cash-out | TBD | | REQ-14, REQ-15 | Planned |
| Settle Up | TBD | | REQ-16, REQ-17, REQ-18 | Planned |

One of the four TBD rows is the shared use case. Decide at the next team
meeting and fill in the owners and branches here.

## Classes

### `Money` — utility, static only (`money.hpp`)

| Member | Status |
| --- | --- |
| `+parseCents(text: string): optional<Cents>` | Implemented |
| `+formatCents(cents: Cents): string` | Implemented |

In code these are free functions in `namespace pokertab`; the diagram shows
them as a utility class.

### `Transaction` — struct (`transaction.hpp`)

| Member | Status |
| --- | --- |
| `+id: int` | Implemented |
| `+timestamp: string` | Implemented |
| `+amountCents: Cents` | Implemented |

### `Player` — struct (`player.hpp`)

| Member | Status |
| --- | --- |
| `+id: int` | Implemented |
| `+name: string` | Implemented |
| `+transactions: vector<Transaction>` | Implemented |
| `+cashOutCents: optional<Cents>` | Implemented |
| `+totalInCents(): Cents` | Implemented |
| `+buyInCount(): int` | Implemented |
| `+cashedOut(): bool` | Implemented |
| `+netCents(): Cents` | Implemented |

### `AddPlayerError` — enum (`session.hpp`)

`EmptyName`, `DuplicateName`. Implemented.

### `Session` (`session.hpp`)

| Member | Status |
| --- | --- |
| `-name_: string` | Implemented |
| `-date_: string` | Implemented |
| `-defaultBuyInCents_: Cents` | Implemented |
| `-players_: vector<Player>` | Implemented |
| `-nextPlayerId_: int` | Implemented |
| `-nextTransactionId_: int` | Implemented |
| `+create(name: string): Session` {static} | Implemented |
| `+create(name: string, date: string): Session` {static} | Implemented |
| `+name(): string` | Implemented |
| `+date(): string` | Implemented |
| `+defaultBuyInCents(): Cents` | Implemented |
| `+setDefaultBuyInCents(c: Cents)` | Implemented |
| `+players(): vector<Player>` | Implemented |
| `+addPlayer(name: string): optional<AddPlayerError>` | Implemented |
| `+findPlayer(id: int): Player*` | Implemented |
| `+potCents(): Cents` | Implemented |
| `+recordBuyIn(playerId: int, cents: Cents): optional<BuyInError>` | Planned — Record Buy-in owner |
| `+recordCashOut(playerId: int, cents: Cents): optional<CashOutError>` | Planned — Record Cash-out owner |
| `+undoLast(): bool` | Planned — REQ-22 |
| `+cashOutTotalCents(): Cents` | Planned — Settle Up owner |
| `+isBalanced(): bool` | Planned — Settle Up owner |

`BuyInError` (`NoSuchPlayer`, `NotPositive`, `PlayerCashedOut`) and
`CashOutError` (`NoSuchPlayer`, `Negative`, `AlreadyCashedOut`) are planned
enums beside `AddPlayerError`.

### `SessionStore` — static only (`session_store.hpp`)

| Member | Status |
| --- | --- |
| `+kSchemaVersion: int = 1` {static} | Implemented |
| `+kFileExtension: string = ".pokertab.json"` {static} | Implemented |
| `+toJson(session: Session): string` {static} | Implemented |
| `+fromJson(json: string): Session` {static} | Implemented |
| `+save(session: Session, path: path)` {static} | Implemented |
| `+load(path: path): Session` {static} | Implemented |
| `+defaultSessionsDir(): path` {static} | Implemented |
| `+pathFor(session: Session): path` {static} | Implemented |

### `Settlement` — planned (`settlement.hpp`)

| Member | Status |
| --- | --- |
| `Payment { payer: string, payee: string, amountCents: Cents }` | Planned — Settle Up owner |
| `+compute(session: Session): vector<Payment>` {static} | Planned — Settle Up owner |

Greedy largest-debtor-to-largest-creditor algorithm (RAD Appendix A).

### `Dashboard` — boundary (`app/dashboard.hpp`)

The one ImGui screen. Owns the current session; every handler calls core
and then saves.

| Member | Status |
| --- | --- |
| `-session_: optional<Session>` | Implemented |
| `-sessionPath_: path` | Implemented |
| `-newName_: string`, `-newDefaultBuyIn_: string`, `-newError_: string` | Implemented |
| `-playerInput_: string`, `-playerError_: string` | Implemented |
| `-status_: string`, `-statusUntil_: double` | Implemented |
| `+draw(nowSeconds: double)` | Implemented |
| `+onCreateSession(name: string, defaultBuyIn: string)` | Implemented |
| `+onOpenRecent(path: path)` | Implemented |
| `+onAddPlayer(name: string)` | Implemented |
| `+onBuyIn(playerId: int, amount: string)` | Stub — Record Buy-in owner |
| `+onCashOut(playerId: int, amount: string)` | Stub — Record Cash-out owner |
| `+onUndo()` | Stub — REQ-22 |
| `+onSettle()` | Stub — Settle Up owner |
| `-persist()` | Implemented |
| `-showStatus(text: string)` | Implemented |

## Relationships

| From | To | Kind | Multiplicity |
| --- | --- | --- | --- |
| Session | Player | composition | 1 to 0..* |
| Player | Transaction | composition | 1 to 0..* |
| Session | AddPlayerError | dependency (returns) | |
| SessionStore | Session | dependency (reads and writes) | |
| Settlement | Session | dependency (reads) | |
| Settlement | Payment | composition (produces) | 1 to 0..* |
| Dashboard | Session | association (owns the current one) | 1 to 0..1 |
| Dashboard | SessionStore | dependency (calls) | |
| Dashboard | Settlement | dependency (calls) | |
| Dashboard | Money | dependency (calls) | |
