# Naming Contract

The class, attribute, and method names every diagram and every source file
must use. Source of truth is `core/include/pokertab/*.hpp`; this file
records the names and marks what is implemented versus planned. If a name
changes in code, change it here and in every diagram in the same pull
request.

Convention: classes `PascalCase`, methods and attributes `camelCase`,
private members end in `_`, money is always `Cents` (int64, whole cents).

## Actors

| Actor | Who |
| --- | --- |
| Accountant | The one person operating Poker Tab at the table (primary actor, every use case) |
| Player | Anyone at the table; reads the settlement screen, operates nothing (secondary actor, Settle Up only) |

## Use cases

| Use case | Owner | REQ | Status |
| --- | --- | --- | --- |
| Create Session | Travis Trinidad | REQ-1, REQ-2 | Implemented |
| Add Player | David Martindale | REQ-5, REQ-6 | Implemented |
| Record Buy-in | Matthew Rohrer | REQ-9, REQ-10, REQ-11 | Planned |
| Record Cash-out | Nishanth Mahendran | REQ-14, REQ-15 | Planned |
| Settle Up | Shared | REQ-16, REQ-17, REQ-18 | Planned |

## Classes

### `Money` — utility, static only (`money.hpp`)

| Member | Status |
| --- | --- |
| `+parseCents(text: string): optional<Cents>` | Implemented |
| `+formatCents(cents: Cents): string` | Implemented |

In code these are free functions in `namespace pokertab`; the diagram shows
them as a utility class.

### `Transaction` — struct (`session.hpp`)

| Member | Status |
| --- | --- |
| `+id: int` | Implemented |
| `+timestamp: string` | Implemented |
| `+amountCents: Cents` | Implemented |

### `Player` — struct (`session.hpp`)

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
| `+recordBuyIn(playerId: int, cents: Cents): optional<BuyInError>` | Planned — Matthew |
| `+recordCashOut(playerId: int, cents: Cents): optional<CashOutError>` | Planned — Nishanth |
| `+undoLast(): bool` | Planned — Nishanth (REQ-22) |
| `+cashOutTotalCents(): Cents` | Planned — shared |
| `+isBalanced(): bool` | Planned — shared |

`BuyInError` (`NoSuchPlayer`, `NotPositive`, `PlayerCashedOut`) and
`CashOutError` (`NoSuchPlayer`, `Negative`, `AlreadyCashedOut`) are planned
enums beside `AddPlayerError`.

### `SessionStore` — static only (`session_store.hpp`)

| Member | Status |
| --- | --- |
| `+toJson(session: Session): string` {static} | Implemented |
| `+fromJson(json: string): Session` {static} | Implemented |
| `+save(session: Session, path: path)` {static} | Implemented |
| `+load(path: path): Session` {static} | Implemented |
| `+defaultSessionsDir(): path` {static} | Implemented |
| `+pathFor(session: Session): path` {static} | Implemented |

### `Settlement` — planned (`settlement.hpp`)

| Member | Status |
| --- | --- |
| `Payment { payer: string, payee: string, amountCents: Cents }` | Planned — shared |
| `+compute(session: Session): vector<Payment>` {static} | Planned — shared |

Greedy largest-debtor-to-largest-creditor algorithm (RAD Appendix A).

### `Dashboard` — boundary (`app/main.cpp`)

The ImGui screen. In code it is the `App` struct plus the free functions in
`main.cpp`; the diagram shows them as one class.

| Member | Status |
| --- | --- |
| `-session: optional<Session>` | Implemented |
| `-sessionPath: path` | Implemented |
| `-status: string` | Implemented |
| `+onCreateSession(name: string, defaultBuyIn: string)` | Implemented (`drawNewSessionDialog`) |
| `+onOpenRecent(path: path)` | Implemented (`drawNewSessionDialog`) |
| `+onAddPlayer(name: string)` | Implemented (`addPlayer`) |
| `+onBuyIn(playerId: int, amount: string)` | Planned — Matthew |
| `+onCashOut(playerId: int, amount: string)` | Planned — Nishanth |
| `+onUndo()` | Planned — Nishanth |
| `+onSettle()` | Planned — shared |
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
