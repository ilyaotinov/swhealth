//
// Created by Отинов Илья on 15.11.2024.
//

#ifndef SWCHECK_HTTPHEALTHCHECKER_H
#define SWCHECK_HTTPHEALTHCHECKER_H
#include "HealthChecker.h"
#include <string>
#include <utility>
#include "../http/HttpClient.h"
#include <memory>

class HttpHealthChecker : public HealthChecker {
public:
    HttpHealthChecker(std::string base_url, std::shared_ptr<HttpClient> client)
            : base_url_(std::move(base_url)), client_(std::move(client)) {};
    [[nodiscard]] bool run() const override;
private:
    std::string base_url_;
    std::shared_ptr<HttpClient> client_;
};

#endif //SWCHECK_HTTPHEALTHCHECKER_H
