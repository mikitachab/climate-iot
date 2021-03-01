#include <cstdlib>

#include "ClimateClient.hpp"

ClimateClient::ClimateClient(std::string baseUrl) : baseUrl(baseUrl),
                                                    client(baseUrl.c_str())

{
}

json ClimateClient::getDeviceHistoryJson(std::string deviceName)
{
    const std::string url = "/device/history/" + deviceName;
    std::cout << url << std::endl;
    auto res = client.Get(url.c_str());
    // TODO error check
    return json::parse(res->body);
}
