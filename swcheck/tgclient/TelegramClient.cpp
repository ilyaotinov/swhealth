#include "TelegramClient.h"
#include <spdlog/spdlog.h>
#include <cpr/cpr.h>

void TelegramClient::sendMessage(const std::string& message) const {
    try {
        spdlog::debug("Sending telegram message: {}", message);

        // Формируем URL и параметры запроса
        std::string url = fmt::format("https://api.telegram.org/bot{}/sendMessage", token_);

        // Используем cpr для отправки POST запроса
        auto response = cpr::Post(
                cpr::Url{url},
                cpr::Parameters{
                        {"chat_id", chat_id_},
                        {"text", message},
                        {"parse_mode", "HTML"}  // Поддержка HTML форматирования
                }
        );

        if (response.status_code != 200) {
            spdlog::error(
                    "Failed to send telegram message. Status: {}, Error: {}",
                    response.status_code,
                    response.text
            );

            // Дополнительная диагностика
            if (response.status_code == 0) {
                spdlog::error("Network error: Could not connect to Telegram API");
                spdlog::debug("URL: {}", url);
                spdlog::debug("Token length: {}", token_.length());
                spdlog::debug("Chat ID: {}", chat_id_);
            }
        } else {
            spdlog::debug("Telegram message sent successfully");
        }
    } catch (const std::exception& e) {
        spdlog::error("Exception while sending telegram message: {}", e.what());
    }
}