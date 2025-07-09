#include "services/AuthService.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <random>

AuthService::AuthService(UserRepository &repo) : userRepo(repo) {}

std::string AuthService::hashPassword(const std::string &password)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char *)password.c_str(), password.length(), hash);

    std::ostringstream ss;
    for (unsigned char c : hash)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)c;

    return ss.str();
}

std::string AuthService::generateToken()
{
    static const char alphanum[] = "0123456789abcdef";
    std::mt19937 rng(std::random_device{}());
    std::string token;
    for (int i = 0; i < 32; ++i)
        token += alphanum[rng() % (sizeof(alphanum) - 1)];
    return token;
}

bool AuthService::signup(const std::string &email, const std::string &password, std::string &outToken)
{
    std::string passwordHash = hashPassword(password);
    outToken = generateToken();
    return userRepo.createUser(email, passwordHash, outToken);
}

bool AuthService::login(const std::string &email, const std::string &password, std::string &outToken)
{
    std::string passwordHash = hashPassword(password);
    return userRepo.validateUser(email, passwordHash, outToken);
}

bool AuthService::isAuthorized(const std::string &email, const std::string &token)
{
    return userRepo.isTokenValid(email, token);
}
