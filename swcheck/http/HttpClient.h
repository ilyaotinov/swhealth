//
// Created by Отинов Илья on 15.11.2024.
//

#ifndef SWCHECK_HTTPCLIENT_H
#define SWCHECK_HTTPCLIENT_H
#include <string>

class HttpClient {
public:
    virtual ~HttpClient() = default;
    [[nodiscard]] virtual long getStatusCode(const std::string& url) const = 0;
};

#endif //SWCHECK_HTTPCLIENT_H
