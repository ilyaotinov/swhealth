#ifndef SWCHECK_CONFIG_H
#define SWCHECK_CONFIG_H

#include <string>
#include <yaml-cpp/yaml.h>

namespace config {
    struct Telegram {
    public:
        std::string token_;
        std::string chat_id_;

        Telegram(std::string token, std::string chat_id)
                : token_(std::move(token))
                , chat_id_(std::move(chat_id)) {}

        static Telegram from_yaml(const YAML::Node &node);
    };

    struct Service {
    public:
        std::string url_;

        explicit Service(std::string url)
                : url_(std::move(url)) {}

        static Service from_yaml(const YAML::Node &node);
    };

    struct App {
    public:
        std::string log_file_path_;

        explicit App(std::string log_file_path)
                : log_file_path_(std::move(log_file_path)) {}

        static App from_yaml(const YAML::Node &node);
    };

    class Config {
    public:
        explicit Config(const std::string &file_path, const std::string &log_path);

    public:
        Telegram telegram_;
        Service service_;
        App app_;
    };
}

#endif //SWCHECK_CONFIG_H