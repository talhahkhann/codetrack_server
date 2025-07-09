#include <nlohmann/json.hpp>
#include "api/AuthHandler.h"

using json = nlohmann::json;

void AuthHandler::registerRoutes(crow::SimpleApp &app, AuthService &authService)
{
    CROW_ROUTE(app, "/auth/signup").methods("POST"_method)([&authService](const crow::request &req)
                                                           {
        try {
            auto body = json::parse(req.body);
            std::string email = body["email"];
            std::string password = body["password"];

            std::string token;
            if (authService.signup(email, password, token)) {
                return crow::response(201, json({ {"status", "success"}, {"token", token} }).dump());
            }
            return crow::response(400, json({ {"status", "error"}, {"message", "Email exists"} }).dump());
        } catch (...) {
            return crow::response(400, R"({"error":"Invalid JSON"})");
        } });

    CROW_ROUTE(app, "/auth/login").methods("POST"_method)([&authService](const crow::request &req)
                                                          {
        try {
            auto body = json::parse(req.body);
            std::string email = body["email"];
            std::string password = body["password"];

            std::string token;
            if(authService.login(email, password, token)) {
                return crow::response(200, json({ {"status", "success"}, {"token", token} }).dump());
            }
            return crow::response(401, json({ {"status", "error"}, {"message", "Invalid credentials"} }).dump());
        } catch (...) {
            return crow::response(400, R"({"error":"Invalid JSON"})");
        } });
}
