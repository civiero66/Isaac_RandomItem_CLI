cmake_minimum_required(VERSION 3.10)
project(Isaac_CLI)

# Используем C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Папка с локальными заголовками
include_directories(include)

# libxml2 через pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)
include_directories(${LIBXML2_INCLUDE_DIRS})
link_directories(${LIBXML2_LIBRARY_DIRS})

# curl
find_package(CURL REQUIRED)

# Собираем все .cpp из src
file(GLOB SOURCES "src/*.cpp")

# Создаем исполняемый файл
add_executable(isaac_cli ${SOURCES})

# Линкуем библиотеки
target_link_libraries(isaac_cli ${LIBXML2_LIBRARIES} CURL::libcurl)