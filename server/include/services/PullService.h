#ifndef PULLSERVICE_HPP
#define PULLSERVICE_HPP

#include <string>
#include <vector>
#include <sqlite3.h>

class PullService
{
public:
    PullService(sqlite3 *db);
    std::vector<std::string> getLatestCommit(const std::string &token);

private:
    sqlite3 *db;
};

#endif // PULLSERVICE_HPP
