cmake_minimum_required(VERSION 3.21)

project(conway LANGUAGES CXX)

find_package(SFML 2 REQUIRED COMPONENTS graphics window system)

add_executable(${PROJECT_NAME} src/main.cpp src/Conway.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)