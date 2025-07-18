#include "crow/crow.h"
#include "db/Database.h"
#include "db/UserRepository.h"
#include "services/AuthService.h"
#include "api/AuthHandler.h"
#include "services/PushService.h"
#include "api/PushHandler.h"
#include "db/CommitRepository.h" // ✅ Added
#include <filesystem>            // ✅ Needed to ensure db folder exists
#include "api/PullHandler.h"
#include "services/PullService.h"
int main()
{
    std::filesystem::create_directories("db"); // ✅ Ensure DB folder exists

    Database db("db/users.db");
    UserRepository userRepo(db);
    userRepo.initializeSchema();
    PullService pullservice(db);

    CommitRepository commitRepo(db); // ✅ Initialize commit repo
    commitRepo.initializeSchema();

    PushService pushService(userRepo, commitRepo); // ✅ Updated to include commitRepo
    AuthService authService(userRepo);

    crow::SimpleApp app;

    AuthHandler::registerRoutes(app, authService);
    PushHandler::registerRoutes(app, pushService);
    PullHandler::registerRoutes(app, pullservice);
    CROW_ROUTE(app, "/")([]
                         { return " CodeTrack Auth Server is running"; });

    app.port(8080).multithreaded().run();
}
