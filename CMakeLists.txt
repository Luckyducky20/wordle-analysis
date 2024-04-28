cmake_minimum_required(VERSION 3.11) # Check the minimum version required for FetchContent
project(wordleEntropy)

set(CMAKE_CXX_STANDARD 17) # Set the C++ standard

include(FetchContent)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib
  GIT_TAG master # or a specific version tag
)

FetchContent_GetProperties(raylib)
if(NOT raylib_POPULATED)
  FetchContent_Populate(raylib)
  add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
endif()

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} raylib)

