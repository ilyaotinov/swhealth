//
// Created by Отинов Илья on 15.11.2024.
//

#include "Container.h"

Container::Container(std::shared_ptr<config::Config> conf)
        : conf_(std::move(conf))
        , http_client_(nullptr)
        , health_checker_(nullptr)
        , telegram_client_(nullptr)
        , service_monitor_(nullptr) {
}

std::shared_ptr<HttpClient> Container::get_http_client() const {
    if (!http_client_) {
        http_client_ = std::make_shared<CprHttpClient>();
    }
    return http_client_;
}

std::shared_ptr<HealthChecker> Container::get_health_checker() const {
    if (!health_checker_) {
        health_checker_ = std::make_shared<HttpHealthChecker>(
                conf_->service_.url_,
                get_http_client()
        );
    }
    return health_checker_;
}

std::shared_ptr<TelegramClient> Container::get_telegram_client() const {
    if (!telegram_client_) {
        telegram_client_ = std::make_shared<TelegramClient>(
                conf_->telegram_.token_,
                conf_->telegram_.chat_id_,
                get_http_client()
        );
    }
    return telegram_client_;
}

std::shared_ptr<ServiceMonitor> Container::get_service_monitor() const {
    if (!service_monitor_) {
        service_monitor_ = std::make_shared<ServiceMonitor>(
                get_health_checker(),
                get_telegram_client()
        );
    }
    return service_monitor_;
}