#include <iostream>
#include <nlohmann/json.hpp>

#include "ClimateHistoryEndpoint.hpp"
#include "utils.hpp"

using json = nlohmann::json;
using namespace Pistache;

ClimateHistoryEndpoint::ClimateHistoryEndpoint(Address addr, HistoryRepository repo) : httpEndpoint(std::make_shared<Http::Endpoint>(addr)),
                                                                         historyRepository(std::make_shared<HistoryRepository>(repo))
{
}

void ClimateHistoryEndpoint::init(size_t thr)
{
    auto opts = Pistache::Http::Endpoint::options()
                    .threads(static_cast<int>(thr));
    httpEndpoint->init(opts);
    setupRoutes();
}

void ClimateHistoryEndpoint::start()
{
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
}

void ClimateHistoryEndpoint::setupRoutes()
{
    using namespace Rest;
    Routes::Get(router, "/device/:device/history", Routes::bind(&ClimateHistoryEndpoint::getDeviceHistoryRecords, this));
    Routes::Post(router, "/device/:device/history", Routes::bind(&ClimateHistoryEndpoint::addDeviceHistoryRecord, this));
}

json makeJsonHistoryRecord(HistoryRecord hr)
{
    json record;
    record["datetime"] = hr.datetime;
    record["temperature"] = utils::toTwoDecimalPlaces(hr.temperature);
    return record;
}

void ClimateHistoryEndpoint::addDeviceHistoryRecord(const Rest::Request &request, Http::ResponseWriter response) {

}

void ClimateHistoryEndpoint::getDeviceHistoryRecords(const Rest::Request &request, Http::ResponseWriter response)
{
    auto deviceName = request.param(":device").as<std::string>();
    auto history = historyRepository->getDeviceHistoryByDeviceName(deviceName);
    if (history.deviceExists)
    {
        json historyJson = json::array();
        for (const auto &hr : history.data)
        {
            historyJson.push_back(makeJsonHistoryRecord(hr));
        }
        response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        response.send(Http::Code::Ok, historyJson.dump());
    }
    else
    {
        json messsage;
        messsage["message"] = "device " + deviceName + " not found";
        response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        response.send(Http::Code::Not_Found, messsage.dump());
    }
}
