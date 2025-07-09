cmake_minimum_required(VERSION 3.10)
project(CodeTrackRemoteAuth)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(include)
include_directories(/home/web-wizard/crow/include)

# Explicitly list source files instead of using GLOB
set(SOURCES
    ./main.cpp
    src/api/AuthHandler.cpp
    src/db/Database.cpp
    src/db/UserRepository.cpp
    src/services/AuthService.cpp
)

add_executable(server ${SOURCES})

target_compile_definitions(server PRIVATE CROW_USE_BOOST)

target_link_libraries(server ${SQLite3_LIBRARIES} OpenSSL::SSL Threads::Threads)