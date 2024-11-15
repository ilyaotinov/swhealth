#include "ServiceMonitor.h"
#include <spdlog/spdlog.h>

void ServiceMonitor::start() {
    bool expected = false;
    if (!is_running_.compare_exchange_strong(expected, true)) {
        spdlog::warn("Monitor is already running");
        return;
    }

    monitor_thread_ = std::make_unique<std::thread>(&ServiceMonitor::monitoringLoop, this);
    spdlog::info("Service monitor started");
}

void ServiceMonitor::stop() {
    is_running_ = false;  // Атомарная операция

    if (monitor_thread_ && monitor_thread_->joinable()) {
        monitor_thread_->join();
        spdlog::info("Service monitor stopped");
    }
}

void ServiceMonitor::monitoringLoop() {
    bool last_status = true;
    spdlog::info("Monitoring loop started");

    while (is_running_) {
        try {
            bool current_status = checker_->run();

            if (last_status && !current_status) {
                telegram_->sendMessage("❌ Белоснежка упала!");
                spdlog::error("snowhite is down");
            } else if (!last_status && current_status) {
                telegram_->sendMessage("✅ Белоснежка снова работает");
                spdlog::info("Service is back online");
            }

            last_status = current_status;

        } catch (const std::exception& e) {
            spdlog::error("Error in monitoring loop: {}", e.what());
        }

        std::this_thread::sleep_for(interval_);
    }

    spdlog::info("Monitoring loop finished");
}