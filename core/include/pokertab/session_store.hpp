// ============================================================================
// session_store.hpp — local JSON persistence for a Session
// ----------------------------------------------------------------------------
// One session per file (RAD 2.1, Appendix B). Amounts are written as integer
// cents. Supports REQ-2 (save) and REQ-3 (load); REQ-4 autosave is the
// caller saving after every change.
//
// File shape (schema 1):
//   {
//     "schema": 1,
//     "name": "Friday night", "date": "2026-09-22",
//     "default_buy_in_cents": 2000,
//     "next_player_id": 3, "next_transaction_id": 1,
//     "players": [
//       { "id": 1, "name": "Alice", "cash_out_cents": null,
//         "transactions": [ { "id": 1, "timestamp": "...", "amount_cents": 2000 } ] }
//     ]
//   }
// ============================================================================
#pragma once

#include <filesystem>
#include <string>

#include "pokertab/session.hpp"

namespace pokertab {

class SessionStore {
public:
    static constexpr int kSchemaVersion = 1;

    // Serialize to / from a JSON string (no filesystem; used by tests and I/O).
    static std::string toJson(const Session& session);
    // Throws std::runtime_error on malformed input.
    static Session fromJson(const std::string& json);

    // REQ-2: write the session to `path`, creating parent directories.
    // Writes to a temp file and renames so a crash never leaves a half file.
    static void save(const Session& session, const std::filesystem::path& path);
    // REQ-3: read a session back. Throws std::runtime_error if unreadable.
    static Session load(const std::filesystem::path& path);

    // Default location: <sessions dir>/<sanitized name>.pokertab.json
    static std::filesystem::path defaultSessionsDir();
    static std::filesystem::path pathFor(const Session& session);
};

}  // namespace pokertab
