#include "PullService.hpp"
#include <iostream>

PullService::PullService(sqlite3 *db) : db(db) {}

std::vector<std::string> PullService::getLatestCommit(const std::string &token)
{
    sqlite3_stmt *stmt;
    const char *sql = R"(
        SELECT filename, content FROM commits
        WHERE user_token = ?
        ORDER BY timestamp DESC
        LIMIT 1;
    )";

    std::vector<std::string> result;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, token.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result.push_back(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
            result.push_back(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        std::cerr << "SQL error in getLatestCommit: " << sqlite3_errmsg(db) << std::endl;
    }

    return result;
}
