cmake_minimum_required(VERSION 3.14)

project(cpp-atimic-operations-tasks LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCES_TASK_01 "task-01/src/*.cpp")
add_executable(task-01 ${SOURCES_TASK_01})

target_include_directories(task-01 PRIVATE ${CMAKE_SOURCE_DIR}/task-01/include)
file(GLOB_RECURSE HEADERS_TASK_01 "task-01/include/*.h")
target_sources(task-01 PRIVATE ${HEADERS_TASK_01})
