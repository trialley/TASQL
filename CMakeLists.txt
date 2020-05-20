cmake_minimum_required (VERSION 2.8.7)
project (TASQL)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(SOURCES_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
set(INCLUDES_DIRECTORY ${PROJECT_SOURCE_DIR}/include)

include_directories(${INCLUDES_DIRECTORY})

#set(SRC_DIRECTORY ${SOURCES_DIRECTORY}/)
#foreach(dir ${SRC_DIRECTORY})
# get directory sources and headers
# set sources
#endforeach()

file(GLOB s_${SOURCES_DIRECTORY} "${SOURCES_DIRECTORY}/*.cpp")
file(GLOB h_${SOURCES_DIRECTORY} "${SOURCES_DIRECTORY}/*.h")
set(TASQL_sources ${s_${SOURCES_DIRECTORY}} ${h_${SOURCES_DIRECTORY}})


add_executable(TASQL ${TASQL_sources})