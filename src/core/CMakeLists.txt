cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME sge_core)
message("==== ${PROJECT_NAME} ====")
project(${PROJECT_NAME})

sge_add_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER sge/core)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt EASTL)