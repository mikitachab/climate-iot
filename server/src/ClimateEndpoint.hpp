#pragma once
#include <memory>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "HistoryRepository.hpp"

using namespace Pistache;

class ClimateEndpoint
{
public:
    explicit ClimateEndpoint(Address, HistoryRepository);
    void init(size_t thr = 2);
    void start();

private:
    void setupRoutes();

    void getDeviceState(const Rest::Request &, Http::ResponseWriter);
    void getDeviceHistory(const Rest::Request &, Http::ResponseWriter);
    void setDeviceDisplay(const Rest::Request &, Http::ResponseWriter);

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    std::shared_ptr<HistoryRepository> historyRepository;
    Rest::Router router;
};
