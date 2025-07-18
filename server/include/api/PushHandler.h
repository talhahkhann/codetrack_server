#pragma once
#include <crow/crow.h>
#include "services/PushService.h"

class PushHandler
{
public:
    static void registerRoutes(crow::SimpleApp &app, PushService &pushService);
};
