# ============================================================================
# PokerTab — root build
# ----------------------------------------------------------------------------
#   cmake -S . -B build                 # configure (GUI on by default)
#   cmake --build build                 # builds core, tests, and the app
#   ctest --test-dir build --output-on-failure
#
#   -DPOKERTAB_BUILD_GUI=OFF            # core + tests only (what CI runs)
# ============================================================================
cmake_minimum_required(VERSION 3.16)
project(PokerTab VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(POKERTAB_BUILD_GUI   "Build the Dear ImGui desktop app" ON)
option(POKERTAB_BUILD_TESTS "Build the Catch2 test suite"       ON)

if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ---- third party (vendored, header-only) -----------------------------------
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/json)

# ---- core: UI-independent session logic (NFR-12) ---------------------------
add_library(pokertab_core STATIC
    core/src/money.cpp
    core/src/session.cpp
    core/src/session_store.cpp
)
target_include_directories(pokertab_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core/include)
target_link_libraries(pokertab_core PRIVATE nlohmann_json)

# ---- tests -----------------------------------------------------------------
if(POKERTAB_BUILD_TESTS)
    enable_testing()
    add_library(catch2 STATIC third_party/catch2/catch_amalgamated.cpp)
    target_include_directories(catch2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/catch2)

    add_executable(pokertab_tests tests/test_session.cpp)
    target_link_libraries(pokertab_tests PRIVATE pokertab_core catch2)
    add_test(NAME pokertab_tests COMMAND pokertab_tests --reporter compact)
endif()

# ---- desktop app -----------------------------------------------------------
if(POKERTAB_BUILD_GUI)
    add_subdirectory(app)
endif()
