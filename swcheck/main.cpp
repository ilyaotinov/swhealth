#include <CLI/CLI.hpp>
#include "config/Config.h"
#include <spdlog/spdlog.h>
#include "spdlog/sinks/basic_file_sink.h"
#include "container/Container.h"
#include <memory>
#include <csignal>
#include <condition_variable>
#include "spdlog/sinks/stdout_color_sinks.h"

void setup_logger(const std::string &log_file_path) {
    try {
        // Создаем вектор приемников логов
        auto sinks = std::vector<spdlog::sink_ptr>();

        // Добавляем цветной вывод в консоль
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::debug);
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        sinks.push_back(console_sink);

        // Добавляем запись в файл
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file_path, true);
        file_sink->set_level(spdlog::level::debug);
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] %v");
        sinks.push_back(file_sink);

        // Создаем логгер с несколькими приемниками
        auto logger = std::make_shared<spdlog::logger>("snowhite-health", sinks.begin(), sinks.end());
        logger->set_level(spdlog::level::debug);

        // Устанавливаем как логгер по умолчанию
        spdlog::set_default_logger(logger);

        spdlog::info("Logger initialized with console and file output");
    }
    catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
        throw;
    }
}

std::shared_ptr<ServiceMonitor> g_monitor;
std::condition_variable g_exit_signal;
std::mutex g_mutex;
bool g_shutdown = false;

void signal_handler(int signal) {
    spdlog::info("Received signal: {}", signal);
    spdlog::info("Shutting down...");

    if (g_monitor) {
        g_monitor->stop();  // Сначала останавливаем
        g_monitor.reset();  // Затем освобождаем
    }

    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_shutdown = true;
    }
    g_exit_signal.notify_one();
}

int main(int argc, char **argv) {
    try {
        CLI::App app{"Service Health Monitor"};
        std::string filename;
        std::string logfile;

        app.add_option("-f,--conf-file", filename, "Configuration file path")->required();
        app.add_option("-l,--log-file", logfile, "Path to log file")->required();

        CLI11_PARSE(app, argc, argv)

        auto conf = std::make_shared<config::Config>(
                app.get_option("-f")->as<std::string>(),
                app.get_option("-l")->as<std::string>()
        );

        setup_logger(conf->app_.log_file_path_);
        spdlog::info("Application initialized");

        auto container = std::make_shared<Container>(conf);
        g_monitor = container->get_service_monitor();

        spdlog::info("Starting service monitor...");
        g_monitor->start();

        std::signal(SIGINT, signal_handler);
        std::signal(SIGTERM, signal_handler);

        {
            std::unique_lock<std::mutex> lock(g_mutex);
            g_exit_signal.wait(lock, [] { return g_shutdown; });
        }

        // Корректное завершение монитора
        if (g_monitor) {
            g_monitor->stop();
            g_monitor.reset();
        }

        spdlog::info("Application shutdown complete");
        return 0;

    } catch (const std::exception& e) {
        if (g_monitor) {
            g_monitor->stop();
            g_monitor.reset();
        }
        spdlog::error("Fatal error: {}", e.what());
        return 1;
    }
}