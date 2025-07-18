#pragma once
#include "db/Database.h"
#include <string>

class CommitRepository
{
public:
    CommitRepository(Database &db);
    void initializeSchema();

    bool saveCommit(
        const std::string &email,
        const std::string &filename,
        const std::string &message,
        const std::string &timestamp,
        const std::string &filesJson,
        const std::string &fullContentJson);

private:
    Database &db;
};
