#ifndef SWCHECK_TELEGRAMCLIENT_H
#define SWCHECK_TELEGRAMCLIENT_H

#include <string>
#include <memory>
#include "../http/HttpClient.h"

class TelegramClient {
public:
    TelegramClient(std::string token, std::string chat_id, std::shared_ptr<HttpClient> client)
            : token_(std::move(token))
            , chat_id_(std::move(chat_id))
            , client_(std::move(client)) {}

    void sendMessage(const std::string& message) const;

private:
    std::string token_;
    std::string chat_id_;
    std::shared_ptr<HttpClient> client_;
};

#endif