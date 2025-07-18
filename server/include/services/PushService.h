// include/services/PushService.h
#pragma once
#include "db/UserRepository.h"
#include "db/CommitRepository.h"
#include <string>

class PushService
{
public:
    PushService(UserRepository &userRepo, CommitRepository &commitRepo);
    bool handlePush(const std::string &token, const std::string &filename, const std::string &content);

private:
    UserRepository &userRepo;
    CommitRepository &commitRepo;
};
