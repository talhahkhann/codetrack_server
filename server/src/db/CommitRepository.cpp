#include "db/CommitRepository.h"
#include <sqlite3.h>
#include <iostream>

CommitRepository::CommitRepository(Database &db) : db(db) {}

void CommitRepository::initializeSchema()
{
    const char *sql = R"(
        CREATE TABLE IF NOT EXISTS commits (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            email TEXT NOT NULL,
            filename TEXT NOT NULL,
            message TEXT,
            timestamp TEXT,
            files TEXT,
            content TEXT
        );
    )";

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db.getConnection(), sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "❌ Commit table creation failed: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
    else
    {
        std::cout << "✅ Commit table ready.\n";
    }
}

bool CommitRepository::saveCommit(
    const std::string &email,
    const std::string &filename,
    const std::string &message,
    const std::string &timestamp,
    const std::string &filesJson,
    const std::string &fullContentJson)
{
    const char *sql = R"(
        INSERT INTO commits (email, filename, message, timestamp, files, content)
        VALUES (?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt *stmt;
    auto conn = db.getConnection();
    if (sqlite3_prepare_v2(conn, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, filename.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, message.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, timestamp.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, filesJson.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, fullContentJson.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return success;
}
