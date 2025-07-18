#include "db/Database.h"
#include <iostream>

Database::Database(const std::string &dbPath)
{
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK)
    {
        std::cerr << "❌ Failed to open DB: " << sqlite3_errmsg(db) << "\n";
        db = nullptr;
    }
    else
    {
        std::cout << "✅ Database connected at: " << dbPath << "\n";
    }
}

Database::~Database()
{
    if (db)
    {
        sqlite3_close(db);
    }
}

sqlite3 *Database::getConnection() const
{
    return db;
}
