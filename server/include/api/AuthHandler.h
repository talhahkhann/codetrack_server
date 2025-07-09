#pragma once
#include "crow/crow.h" // ✅ correct for your current setup
#include "services/AuthService.h"

class AuthHandler
{
public:
    static void registerRoutes(crow::SimpleApp &app, AuthService &authService);
};
