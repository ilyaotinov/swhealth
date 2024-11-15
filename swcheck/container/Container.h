//
// Created by Отинов Илья on 15.11.2024.
//

#ifndef SWCHECK_CONTAINER_H
#define SWCHECK_CONTAINER_H

#include "../http/HttpClient.h"
#include "../http/CprHttpClient.h"
#include "../config/Config.h"
#include <memory>
#include "../healthcheck/HttpHealthChecker.h"
#include "../tgclient/TelegramClient.h"
#include "../monitor/ServiceMonitor.h"

class Container {
public:
    explicit Container(std::shared_ptr<config::Config> conf);

    [[nodiscard]] std::shared_ptr<HttpClient> get_http_client() const;
    [[nodiscard]] std::shared_ptr<HealthChecker> get_health_checker() const;
    [[nodiscard]] std::shared_ptr<TelegramClient> get_telegram_client() const;
    [[nodiscard]] std::shared_ptr<ServiceMonitor> get_service_monitor() const;

private:
    std::shared_ptr<config::Config> conf_;
    mutable std::shared_ptr<HttpClient> http_client_;
    mutable std::shared_ptr<HealthChecker> health_checker_;
    mutable std::shared_ptr<TelegramClient> telegram_client_;
    mutable std::shared_ptr<ServiceMonitor> service_monitor_;
};


#endif //SWCHECK_CONTAINER_H
