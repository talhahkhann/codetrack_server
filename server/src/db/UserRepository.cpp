#include "db/UserRepository.h"
#include <sqlite3.h>
#include <iostream>

UserRepository::UserRepository(Database &db) : db(db) {}

void UserRepository::initializeSchema()
{
    const char *query = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            email TEXT UNIQUE NOT NULL,
            password_hash TEXT NOT NULL,
            token TEXT NOT NULL
        );
    )";
    char *errMsg = nullptr;
    int rc = sqlite3_exec(db.getConnection(), query, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "DB schema error: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}

bool UserRepository::createUser(const std::string &email, const std::string &passwordHash, const std::string &token)
{
    const char *sql = "INSERT INTO users (email, password_hash, token) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;
    auto conn = db.getConnection();
    if (sqlite3_prepare_v2(conn, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, passwordHash.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, token.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return success;
}

bool UserRepository::validateUser(const std::string &email, const std::string &passwordHash, std::string &outToken)
{
    const char *sql = "SELECT token FROM users WHERE email = ? AND password_hash = ?;";
    sqlite3_stmt *stmt;
    auto conn = db.getConnection();
    if (sqlite3_prepare_v2(conn, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, passwordHash.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        outToken = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        sqlite3_finalize(stmt);
        return true;
    }

    sqlite3_finalize(stmt);
    return false;
}

bool UserRepository::isTokenValid(const std::string &email, const std::string &token)
{
    const char *sql = "SELECT 1 FROM users WHERE email = ? AND token = ?;";
    sqlite3_stmt *stmt;
    auto conn = db.getConnection();
    if (sqlite3_prepare_v2(conn, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, token.c_str(), -1, SQLITE_TRANSIENT);

    bool valid = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return valid;
}

bool UserRepository::getEmailByToken(const std::string &token, std::string &emailOut)
{
    const char *sql = "SELECT email FROM users WHERE token = ?";
    sqlite3_stmt *stmt;
    auto conn = db.getConnection();

    if (sqlite3_prepare_v2(conn, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, token.c_str(), -1, SQLITE_TRANSIENT);

    bool found = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        emailOut = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
        found = true;
    }

    sqlite3_finalize(stmt);
    return found;
}
