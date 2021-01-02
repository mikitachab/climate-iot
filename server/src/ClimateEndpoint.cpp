#include <iostream>
#include <nlohmann/json.hpp>

#include "ClimateEndpoint.hpp"
#include "utils.hpp"

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
    Routes::Get(router, "/device/history/:device", Routes::bind(&ClimateEndpoint::getDeviceHistory, this));
    Routes::Post(router, "/device/display/", Routes::bind(&ClimateEndpoint::setDeviceDisplay, this));
}

json makeJsonHistoryRecord(HistoryRecord hr)
{
    json record;
    record["datetime"] = hr.datetime;
    record["temperature"] = utils::toTwoDecimalPlaces(hr.temperature);
    return record;
}

void ClimateEndpoint::getDeviceHistory(const Rest::Request &request, Http::ResponseWriter response)
{
    auto deviceName = request.param(":device").as<std::string>();
    auto history = historyRepository->getDeviceHistoryByDeviceName(deviceName);
    json historyJson = json::array();
    for (const auto &hr : history.data)
    {
        historyJson.push_back(makeJsonHistoryRecord(hr));
    }
    response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
    response.send(Http::Code::Ok, historyJson.dump());
}

void ClimateEndpoint::setDeviceDisplay(const Rest::Request &request, Http::ResponseWriter response)
{
    std::cout << request.body() << std::endl;
    response.send(Http::Code::Ok, "set device display");
}

void ClimateEndpoint::getDeviceState(const Rest::Request &request, Http::ResponseWriter response)
{
    response.send(Http::Code::Ok, "get device state");
}
