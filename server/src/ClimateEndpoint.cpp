#include <iostream>
#include <nlohmann/json.hpp>

#include "ClimateEndpoint.hpp"

using json = nlohmann::json;
using namespace Pistache;

ClimateEndpoint::ClimateEndpoint(Address addr, HistoryRepository repo) : httpEndpoint(std::make_shared<Http::Endpoint>(addr)),
                                                                         historyRepository(std::make_shared<HistoryRepository>(repo))
{
}

void ClimateEndpoint::init(size_t thr)
{
    auto opts = Pistache::Http::Endpoint::options()
                    .threads(static_cast<int>(thr));
    httpEndpoint->init(opts);
    setupRoutes();
}

void ClimateEndpoint::start()
{
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
}

void ClimateEndpoint::setupRoutes()
{
    using namespace Rest;
    Routes::Get(router, "/device/state/", Routes::bind(&ClimateEndpoint::getDeviceState, this));
    Routes::Get(router, "/device/history/", Routes::bind(&ClimateEndpoint::getDeviceHistory, this));
    Routes::Post(router, "/device/display/", Routes::bind(&ClimateEndpoint::setDeviceDisplay, this));
}

void ClimateEndpoint::getDeviceState(const Rest::Request &request, Http::ResponseWriter response)
{
    response.send(Http::Code::Ok, "get device state");
}

double ToTwoDecimalPlaces(double d) // TODO move to utils or similar
{

    int i;
    if (d >= 0)
        i = static_cast<int>(d * 100 + 0.5);
    else
        i = static_cast<int>(d * 100 - 0.5);
    return (i / 100.0);
}

void ClimateEndpoint::getDeviceHistory(const Rest::Request &request, Http::ResponseWriter response)
{
    json historyJson = json::array();
    auto history = historyRepository->getDeviceHistoryByDeviceName("device-0");
    for (const auto &h : history.data)
    {
        json record;
        record["datetime"] = h.datetime;
        record["temperature"] = ToTwoDecimalPlaces(h.temperature);
        historyJson.push_back(record);
    }
    response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
    response.send(Http::Code::Ok, historyJson.dump());
}

void ClimateEndpoint::setDeviceDisplay(const Rest::Request &request, Http::ResponseWriter response)
{
    std::cout << request.body() << std::endl;
    response.send(Http::Code::Ok, "set device display");
}
