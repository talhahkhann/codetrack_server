#include "crow/crow.h"
#include "db/Database.h"
#include "db/UserRepository.h"
#include "services/AuthService.h"
#include "api/AuthHandler.h"
int main()
{
    Database db("db/users.db");
    UserRepository userRepo(db);
    userRepo.initializeSchema();

    AuthService authService(userRepo);

    crow::SimpleApp app;

    AuthHandler::registerRoutes(app, authService);

    CROW_ROUTE(app, "/")([]
                         { return " CodeTrack Auth Server is running"; });

    app.port(8080).multithreaded().run();
}