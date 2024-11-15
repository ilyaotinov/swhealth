cmake_minimum_required(VERSION 3.27)
project(swcheck)

find_package(yaml-cpp REQUIRED)
find_package(CLI11 REQUIRED)
find_package(spdlog REQUIRED)
find_package(cpr REQUIRED)
find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 23)

# Основная библиотека
add_library(swcheck_lib
        swcheck/config/Config.cpp
        swcheck/config/Config.h
        swcheck/healthcheck/HealthChecker.h
        swcheck/healthcheck/HttpHealthChecker.cpp
        swcheck/healthcheck/HttpHealthChecker.h
        swcheck/http/HttpClient.h
        swcheck/http/CprHttpClient.cpp
        swcheck/http/CprHttpClient.h
)

target_link_libraries(swcheck_lib
        PUBLIC
        yaml-cpp
        CLI11::CLI11
        spdlog::spdlog
        cpr::cpr
)

# Основной исполняемый файл
add_executable(swcheck swcheck/main.cpp
        swcheck/container/Container.cpp
        swcheck/container/Container.h
        swcheck/tgclient/TelegramClient.cpp
        swcheck/tgclient/TelegramClient.h
        swcheck/monitor/ServiceMonitor.cpp
        swcheck/monitor/ServiceMonitor.h)
target_link_libraries(swcheck PRIVATE swcheck_lib)

# Тесты
enable_testing()
add_executable(unit_tests
        swcheck/healthcheck/HttpHealthCheckerTest.cpp
)

target_link_libraries(unit_tests
        PRIVATE
        swcheck_lib
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)