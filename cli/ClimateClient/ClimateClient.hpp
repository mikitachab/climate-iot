#pragma once

#include <string>
#include <httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string CLIMATE_SERVER_URL = std::getenv("CLIMATE_SERVER_URL");

class ClimateClient
{
public:
    ClimateClient(std::string baseUrl = CLIMATE_SERVER_URL);
    json getDeviceHistoryJson(std::string);

private:
    std::string baseUrl;
    httplib::Client client;
};
