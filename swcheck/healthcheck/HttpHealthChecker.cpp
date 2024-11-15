//
// Created by Отинов Илья on 15.11.2024.
//

#include "HttpHealthChecker.h"
#include <cpr/cpr.h>

bool HttpHealthChecker::run() const {
    auto code = client_->getStatusCode(base_url_);
    return code >= 200 && code < 300;
}
