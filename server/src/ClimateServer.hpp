#pragma once
#include <memory>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "HistoryRepository.hpp"

using namespace Pistache;

class ClimateServer
{
public:
    explicit ClimateServer(Address, std::shared_ptr<IClimateRepository>);
    void init(size_t thr = 2);
    void start();

private:
    void setupRoutes();

    void getDeviceHistoryRecords(const Rest::Request &, Http::ResponseWriter);
    void addDeviceHistoryRecord(const Rest::Request &, Http::ResponseWriter);
    void addDevice(const Rest::Request &, Http::ResponseWriter);
    void getDevices(const Rest::Request &, Http::ResponseWriter);
    void ping(const Rest::Request &, Http::ResponseWriter);

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    std::shared_ptr<IClimateRepository> historyRepository;
    Rest::Router router;
};
