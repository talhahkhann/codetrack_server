#include "PullHandler.hpp"

void PullHandler::registerRoutes(crow::SimpleApp &app, PullService &pullService)
{
    CROW_ROUTE(app, "/pull")
        .methods("GET"_method)([&pullService](const crow::request &req)
                               {
        auto token = req.url_params.get("token");

        if (!token) {
            return crow::response(400, "Missing token");
        }

        auto latestCommit = pullService.getLatestCommit(token);
        if (latestCommit.empty()) {
            return crow::response(404, "No commits found for user");
        }

        crow::json::wvalue res;
        res["filename"] = latestCommit[0];
        res["content"] = latestCommit[1];
        return crow::response{res}; });
}
