//
// Created by Отинов Илья on 15.11.2024.
//

#ifndef SWCHECK_CPRHTTPCLIENT_H
#define SWCHECK_CPRHTTPCLIENT_H

#include "HttpClient.h"

class CprHttpClient : public HttpClient {
public:
    long getStatusCode(const std::string& url) const override;
};

#endif //SWCHECK_CPRHTTPCLIENT_H
