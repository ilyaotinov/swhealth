#include "Config.h"
#include <yaml-cpp/yaml.h>

config::Config::Config(const std::string &file_path, const std::string &log_path)
        : telegram_("", "")
        , service_("")
        , app_("") {
    YAML::Node yaml_config = YAML::LoadFile(file_path);
    telegram_ = Telegram::from_yaml(yaml_config["telegram"]);
    service_ = Service::from_yaml(yaml_config["service"]);
    app_ = App::from_yaml(yaml_config["app"]);
}

config::App config::App::from_yaml(const YAML::Node &node) {
    if (!node || !node["log_path"]) {
        throw std::runtime_error("Invalid application configuration");
    }

    return config::App(node["log_path"].as<std::string>());
}

config::Telegram config::Telegram::from_yaml(const YAML::Node &node) {
    if (!node) {
        throw std::runtime_error("Invalid telegram configuration: missing section");
    }
    if (!node["token"]) {
        throw std::runtime_error("Invalid telegram configuration: missing token");
    }
    if (!node["chat_id"]) {
        throw std::runtime_error("Invalid telegram configuration: missing chat_id");
    }

    return {
            node["token"].as<std::string>(),
            node["chat_id"].as<std::string>()
    };
}

config::Service config::Service::from_yaml(const YAML::Node &node) {
    if (!node || !node["url"]) {
        throw std::runtime_error("Invalid service configuration: missing url");
    }

    return config::Service(node["url"].as<std::string>());
}