#include "api/PushHandler.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void PushHandler::registerRoutes(crow::SimpleApp &app, PushService &pushService)
{
    CROW_ROUTE(app, "/push").methods("POST"_method)([&pushService](const crow::request &req)
                                                    {
        try {
            std::cout << "📥 Received request body: " << req.body << std::endl;
            
            auto body = json::parse(req.body);
            std::cout << " JSON parsed successfully" << std::endl;

            //  Check if required fields exist
            if (!body.contains("token") || !body.contains("commit")) {
                std::cout << "❌ Missing token or commit data" << std::endl;
                return crow::response(400, json({
                    {"status", "error"},
                    {"message", "Missing token or commit data"}
                }).dump());
            }

            //  Extract token from the main body
            std::string token = body["token"];
            std::cout << "🔑 Token extracted: " << token.substr(0, 8) << "..." << std::endl;
            
            //  Extract commit data
            auto commit = body["commit"];
            
            if (!commit.contains("filename") || !commit.contains("content")) {
                std::cout << "❌ Missing filename or content in commit" << std::endl;
                return crow::response(400, json({
                    {"status", "error"},
                    {"message", "Missing filename or content in commit"}
                }).dump());
            }
            
            std::string filename = commit["filename"];
            std::cout << "📁 Filename: " << filename << std::endl;
            
            //  Convert the content JSON object to string for storage
            std::string content = commit["content"].dump();
            std::cout << "📝 Content size: " << content.length() << " bytes" << std::endl;

            //  Call pushService with the extracted data
            std::cout << "🚀 Calling pushService.handlePush..." << std::endl;
            bool success = pushService.handlePush(token, filename, content);
            std::cout << "📤 PushService result: " << (success ? "SUCCESS" : "FAILED") << std::endl;

            if (success) {
                return crow::response(200, json({
                    {"status", "success"},
                    {"message", "Commit pushed"}
                }).dump());
            } else {
                return crow::response(401, json({
                    {"status", "error"},
                    {"message", "Invalid token"}
                }).dump());
            }
        }
        catch (const json::parse_error& e) {
            std::cout << "❌ JSON parse error: " << e.what() << std::endl;
            return crow::response(400, json({
                {"status", "error"},
                {"message", "Invalid JSON: " + std::string(e.what())}
            }).dump());
        }
        catch (const json::out_of_range& e) {
            std::cout << "❌ JSON out of range error: " << e.what() << std::endl;
            return crow::response(400, json({
                {"status", "error"},
                {"message", "Missing required fields: " + std::string(e.what())}
            }).dump());
        }
        catch (const std::exception& e) {
            std::cout << "❌ Server error: " << e.what() << std::endl;
            return crow::response(500, json({
                {"status", "error"},
                {"message", "Server error: " + std::string(e.what())}
            }).dump());
        }

        return crow::response(500, json({
            {"status", "error"},
            {"message", "Unhandled error"}
        }).dump()); });
}