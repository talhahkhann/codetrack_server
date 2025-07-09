#pragma once
#include "db/Database.h"
#include <string>

class UserRepository
{
public:
    explicit UserRepository(Database &db);
    void initializeSchema();
    bool createUser(const std::string &email, const std::string &passwordHash, const std::string &token);
    bool validateUser(const std::string &email, const std::string &passwordHash, std::string &outToken);
    bool isTokenValid(const std::string &email, const std::string &token);

private:
    Database &db;
};
