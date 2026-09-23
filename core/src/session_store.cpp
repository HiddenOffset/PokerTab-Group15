// ============================================================================
// session_store.cpp
// ============================================================================
#include "pokertab/session_store.hpp"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

namespace pokertab {

using nlohmann::json;

// ---- JSON ------------------------------------------------------------------

std::string SessionStore::toJson(const Session& s) {
    json j;
    j["schema"] = kSchemaVersion;
    j["name"] = s.name();
    j["date"] = s.date();
    j["default_buy_in_cents"] = s.defaultBuyInCents();
    j["next_player_id"] = s.nextPlayerId();
    j["next_transaction_id"] = s.nextTransactionId();
    j["players"] = json::array();
    for (const auto& p : s.players()) {
        json jp;
        jp["id"] = p.id;
        jp["name"] = p.name;
        if (p.cashOutCents) jp["cash_out_cents"] = *p.cashOutCents;
        else jp["cash_out_cents"] = nullptr;
        jp["transactions"] = json::array();
        for (const auto& t : p.transactions) {
            jp["transactions"].push_back({{"id", t.id},
                                          {"timestamp", t.timestamp},
                                          {"amount_cents", t.amountCents}});
        }
        j["players"].push_back(std::move(jp));
    }
    return j.dump(2);
}

Session SessionStore::fromJson(const std::string& text) {
    json j;
    try {
        j = json::parse(text);
    } catch (const json::exception& e) {
        throw std::runtime_error(std::string("session file is not valid JSON: ") + e.what());
    }
    try {
        if (j.at("schema").get<int>() != kSchemaVersion)
            throw std::runtime_error("unsupported session file schema");

        Session s = Session::create(j.at("name").get<std::string>(),
                                    j.at("date").get<std::string>());
        s.defaultBuyInCents_ = j.value("default_buy_in_cents", Cents{2000});
        s.nextPlayerId_ = j.value("next_player_id", 1);
        s.nextTransactionId_ = j.value("next_transaction_id", 1);

        for (const auto& jp : j.at("players")) {
            Player p;
            p.id = jp.at("id").get<int>();
            p.name = jp.at("name").get<std::string>();
            if (jp.contains("cash_out_cents") && !jp["cash_out_cents"].is_null())
                p.cashOutCents = jp["cash_out_cents"].get<Cents>();
            for (const auto& jt : jp.value("transactions", json::array())) {
                Transaction t;
                t.id = jt.at("id").get<int>();
                t.timestamp = jt.value("timestamp", "");
                t.amountCents = jt.at("amount_cents").get<Cents>();
                p.transactions.push_back(std::move(t));
            }
            s.players_.push_back(std::move(p));
        }
        return s;
    } catch (const json::exception& e) {
        throw std::runtime_error(std::string("session file is missing a field: ") + e.what());
    }
}

// ---- Files -----------------------------------------------------------------

void SessionStore::save(const Session& s, const std::filesystem::path& path) {
    namespace fs = std::filesystem;
    if (path.has_parent_path()) fs::create_directories(path.parent_path());

    const fs::path tmp = path.string() + ".tmp";
    {
        std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
        if (!out) throw std::runtime_error("cannot write " + tmp.string());
        out << toJson(s);
        if (!out) throw std::runtime_error("write failed for " + tmp.string());
    }
    fs::rename(tmp, path);  // atomic replace on POSIX and NTFS
}

Session SessionStore::load(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("cannot open " + path.string());
    std::stringstream buf;
    buf << in.rdbuf();
    return fromJson(buf.str());
}

std::filesystem::path SessionStore::defaultSessionsDir() {
    namespace fs = std::filesystem;
#ifdef _WIN32
    if (const char* appdata = std::getenv("APPDATA"))
        return fs::path(appdata) / "PokerTab" / "sessions";
#else
    if (const char* home = std::getenv("HOME"))
        return fs::path(home) / ".pokertab" / "sessions";
#endif
    return fs::current_path() / "sessions";
}

std::filesystem::path SessionStore::pathFor(const Session& s) {
    std::string file;
    for (char c : s.name()) {
        const bool ok = std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_';
        file += ok ? c : '_';
    }
    if (file.empty()) file = "session";
    return defaultSessionsDir() / (s.date() + "_" + file + ".pokertab.json");
}

}  // namespace pokertab
