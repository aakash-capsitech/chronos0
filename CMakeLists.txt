cmake_minimum_required(VERSION 3.10)
project(Chronos)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2 (Required for Client)
find_package(SDL2 REQUIRED)

# Common code shared by both
add_library(common net/socket.cpp)
target_include_directories(common PUBLIC core net)

# Build Server
add_executable(chronos_server server/main.cpp)
target_link_libraries(chronos_server common)

# Build Client
add_executable(chronos_client client/main.cpp)
target_link_libraries(chronos_client common ${SDL2_LIBRARIES})
target_include_directories(chronos_client PRIVATE ${SDL2_INCLUDE_DIRS})