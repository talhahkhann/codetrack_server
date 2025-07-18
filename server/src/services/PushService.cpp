// src/services/PushService.cpp
#include "services/PushService.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

PushService::PushService(UserRepository &userRepo, CommitRepository &commitRepo)
    : userRepo(userRepo), commitRepo(commitRepo) {}

bool PushService::handlePush(const std::string &token, const std::string &filename, const std::string &content)
{
    std::string email;
    std::cout << "🔐 Verifying token: " << token << std::endl;
    if (!userRepo.getEmailByToken(token, email))
    {
        std::cout << "❌ Invalid token" << std::endl;
        return false;
    }

    try
    {
        json parsed = json::parse(content);
        std::string message = parsed.value("message", "No message");
        std::string timestamp = parsed.value("timestamp", "unknown");
        std::string files = parsed.contains("files") ? json(parsed["files"]).dump() : "[]";

        bool saved = commitRepo.saveCommit(email, filename, message, timestamp, files, content);
        if (saved)
        {
            std::cout << "✅ Commit saved to DB for " << email << std::endl;
        }
        return saved;
    }
    catch (const std::exception &e)
    {
        std::cout << "❌ Failed to parse JSON or insert commit: " << e.what() << std::endl;
        return false;
    }
}
