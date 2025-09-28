cmake_minimum_required(VERSION 3.10)
project(Isaac_CLI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)
include_directories(${LIBXML2_INCLUDE_DIRS})
link_directories(${LIBXML2_LIBRARY_DIRS})

find_package(CURL REQUIRED)

file(GLOB SOURCES "src/*.cpp")

add_executable(isaac_cli ${SOURCES})

target_link_libraries(isaac_cli ${LIBXML2_LIBRARIES} CURL::libcurl)
