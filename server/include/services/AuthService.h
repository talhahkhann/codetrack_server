#pragma once
#include "db/UserRepository.h"

class AuthService
{
public:
    explicit AuthService(UserRepository &repo);

    bool signup(const std::string &email, const std::string &password, std::string &outToken);
    bool login(const std::string &email, const std::string &password, std::string &outToken);
    bool isAuthorized(const std::string &email, const std::string &token);

private:
    std::string hashPassword(const std::string &password);
    std::string generateToken();
    UserRepository &userRepo;
};
