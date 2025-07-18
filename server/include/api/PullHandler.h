#ifndef PULLHANDLER_HPP
#define PULLHANDLER_HPP

#include "crow.h"
#include "../services/PullService.hpp"

class PullHandler
{
public:
    static void registerRoutes(crow::SimpleApp &app, PullService &pullService);
};

#endif // PULLHANDLER_HPP
