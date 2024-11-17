cmake_minimum_required(VERSION 3.5.0)

set(VERSION "0.0.1")
set(PROJECT_NAME "Game")

project(${PROJECT_NAME} VERSION ${VERSION} )

include_directories(SYSTEM "include/glad")
include_directories(SYSTEM "include/stb")

add_executable(${PROJECT_NAME} src/getting_started/textures/main.cpp "include/glad/glad.c" "include/stb/stb_image.h")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
target_link_libraries(${PROJECT_NAME} glfw m)
