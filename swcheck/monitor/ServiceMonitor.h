#ifndef SWCHECK_SERVICEMONITOR_H
#define SWCHECK_SERVICEMONITOR_H

#include <memory>
#include <chrono>
#include <thread>
#include <atomic>
#include "../healthcheck/HealthChecker.h"
#include "../tgclient/TelegramClient.h"

class ServiceMonitor {
public:
    ServiceMonitor(
            std::shared_ptr<HealthChecker> checker,
            std::shared_ptr<TelegramClient> telegram,
            std::chrono::seconds interval = std::chrono::seconds(10)  // 30 секунд по умолчанию
    )
            : checker_(std::move(checker))
            , telegram_(std::move(telegram))
            , interval_(interval)
            , is_running_(false) {}

    // Добавляем деструктор
    ~ServiceMonitor() {
        stop();  // Гарантируем остановку потока
    }

    // Запрещаем копирование и перемещение
    ServiceMonitor(const ServiceMonitor&) = delete;
    ServiceMonitor& operator=(const ServiceMonitor&) = delete;

    void start();
    void stop();

private:
    void monitoringLoop();

    std::shared_ptr<HealthChecker> checker_;
    std::shared_ptr<TelegramClient> telegram_;
    std::chrono::seconds interval_;
    std::atomic<bool> is_running_;  // Делаем атомарным
    std::unique_ptr<std::thread> monitor_thread_;
};

#endif