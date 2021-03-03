#include <iostream>
#include <nlohmann/json.hpp>

#include "ClimateServer.hpp"
#include "utils.hpp"

using json = nlohmann::json;
using namespace Pistache;

ClimateServer::ClimateServer(Address addr, HistoryRepository repo) : httpEndpoint(std::make_shared<Http::Endpoint>(addr)),
                                                                         historyRepository(std::make_shared<HistoryRepository>(repo))
{
}

void ClimateServer::init(size_t thr)
{
    auto opts = Pistache::Http::Endpoint::options()
                    .threads(static_cast<int>(thr));
    httpEndpoint->init(opts);
    setupRoutes();
}

void ClimateServer::start()
{
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
}

void ClimateServer::setupRoutes()
{
    using namespace Rest;
    Routes::Post(router, "/devices", Routes::bind(&ClimateServer::addDevice, this));
    Routes::Get(router, "/devices", Routes::bind(&ClimateServer::getDevices, this));
    Routes::Get(router, "/devices/:device/history", Routes::bind(&ClimateServer::getDeviceHistoryRecords, this));
    Routes::Post(router, "/devices/:device/history", Routes::bind(&ClimateServer::addDeviceHistoryRecord, this));
    Routes::Get(router, "/ping", Routes::bind(&ClimateServer::ping, this));
}

json makeJsonHistoryRecord(HistoryRecord hr)
{
    json record;
    record["datetime"] = hr.datetime;
    record["temperature"] = utils::toTwoDecimalPlaces(hr.temperature);
    return record;
}

void ClimateServer::addDeviceHistoryRecord(const Rest::Request &request, Http::ResponseWriter response) {
    auto body = json::parse(request.body());
    auto deviceName = request.param(":device").as<std::string>();
    int id = historyRepository->getDeviceIdByName(deviceName);
    historyRepository->addHistoryRecord(id, body["temperature"], body["datetime"]);
    json record;
    record["temperature"] = body["temperature"];
    record["datetime"] = body["datetime"];
    response.send(Http::Code::Created, record.dump());
}

void ClimateServer::getDeviceHistoryRecords(const Rest::Request &request, Http::ResponseWriter response)
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

void ClimateServer::ping(const Rest::Request &request, Http::ResponseWriter response){
        response.send(Http::Code::Ok, "PONG");
}

void ClimateServer::addDevice(const Rest::Request &request, Http::ResponseWriter response){
    auto body = json::parse(request.body());
    std::string deviceName = body["name"];
    if(!historyRepository->addDevice(deviceName)){
        json device;
        device["name"] = deviceName;
        response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        response.send(Http::Code::Created, device.dump());
    } else {
        response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        response.send(Http::Code::Bad_Request, "{\"message\": \"device " +  deviceName + " already exists\"}");
    }
}

void ClimateServer::getDevices(const Rest::Request &request, Http::ResponseWriter response){
    auto devices =  historyRepository->getAllDevices();
    auto devicesJson = json::array();
    for (auto d: devices) {
        json deviceJson;
        deviceJson["name"] = d.name;
        devicesJson.push_back(deviceJson);
    }
    response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
    response.send(Http::Code::Ok, devicesJson.dump());
}
