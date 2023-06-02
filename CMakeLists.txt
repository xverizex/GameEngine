cmake_minimum_required (VERSION 3.12)

set (PROJECT_NAME Game)

project (${PROJECT_NAME})

add_subdirectory (src)

file (COPY assets DESTINATION ${CMAKE_BINARY_DIR})
