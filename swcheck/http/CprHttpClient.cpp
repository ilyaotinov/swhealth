//
// Created by Отинов Илья on 15.11.2024.
//

#include "CprHttpClient.h"
#include "cpr/cpr.h"

long CprHttpClient::getStatusCode(const std::string &url) const {
    auto r = cpr::Get(cpr::Url(url));

    return r.status_code;
}
