# Developing Poker Tab

## Layout

```
core/         UI-independent session logic (static library, NFR-12)
  include/pokertab/money.hpp          integer-cent money, parse/format
  include/pokertab/session.hpp        Session, Player, Transaction
  include/pokertab/session_store.hpp  JSON save/load
  src/
app/          Dear ImGui desktop dashboard (main.cpp)
tests/        Catch2 tests, one TEST_CASE per REQ acceptance criterion
third_party/  vendored Catch2 v3 and nlohmann/json (no download needed)
docs/         wireframes and design notes
.github/workflows/ci.yml   runs the tests on every pull request
```

Dear ImGui is fetched from GitHub the first time you configure with the GUI
on. GLFW comes from the system if installed, otherwise it is fetched too.

## Build

Prerequisites: CMake 3.16+, a C++17 compiler, git.

```
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Run the app: `build/app/pokertab` (Linux/macOS) or
`build\app\Debug\pokertab.exe` (Visual Studio generator on Windows).

Core and tests only, no GUI dependencies:

```
cmake -S . -B build -DPOKERTAB_BUILD_GUI=OFF
```

### Windows

Install Visual Studio 2022 with "Desktop development with C++" and CMake.
Open the folder in VS Code with the CMake Tools extension, or use the tasks
in `.vscode/tasks.json` (Ctrl+Shift+B builds, Ctrl+Shift+P → "Run Test
Task" runs the tests). GLFW is built from source automatically.

### Linux

```
sudo apt install cmake g++ libglfw3-dev libgl1-mesa-dev
```

### macOS

```
brew install cmake glfw
```

## Where sessions are saved

One JSON file per session (see `session_store.hpp` for the schema):

- Windows: `%APPDATA%\PokerTab\sessions\`
- Linux/macOS: `~/.pokertab/sessions/`

Every change (create session, add player) is written immediately (REQ-4).

## Adding a requirement

1. Add the logic to `core/` — never in `app/main.cpp`. The UI only calls
   core and redraws.
2. Add a `TEST_CASE("REQ-N: ...", "[REQ-N]")` in `tests/test_session.cpp`
   whose checks mirror the acceptance criterion in the RAD.
3. Wire the control in `app/main.cpp`. Disabled placeholders already exist
   for buy-in, cash-out, undo and settle.
4. Open a pull request. CI must be green before merge.

Run one requirement's tests alone:

```
build/pokertab_tests "[REQ-5]"
```

## Test coverage by requirement

| REQ | Test | What it checks |
| --- | --- | --- |
| REQ-1 | `REQ-1: new session has the given name, today's date, no players, $0.00 pot` | Session::create |
| REQ-2, REQ-3 | `REQ-2/REQ-3: save then load restores every player exactly` | SessionStore round trip |
| REQ-3 | `REQ-3: loading a missing or corrupt file throws` | error handling |
| REQ-5 | `REQ-5: adding a player shows them on the roster with $0.00 in and $0.00 out` | Session::addPlayer |
| REQ-6 | `REQ-6: duplicate player name is rejected and roster unchanged` | duplicate check |
| REQ-8 | `REQ-8: at least 10 players fit in one session` | capacity |
| REQ-11 | `money: parse and format whole cents` | input validation groundwork |
