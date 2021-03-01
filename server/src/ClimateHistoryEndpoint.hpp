#pragma once
#include <memory>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "HistoryRepository.hpp"

using namespace Pistache;

class ClimateHistoryEndpoint
{
public:
    explicit ClimateHistoryEndpoint(Address, HistoryRepository);
    void init(size_t thr = 2);
    void start();

private:
    void setupRoutes();

    void getDeviceHistoryRecords(const Rest::Request &, Http::ResponseWriter);
    void addDeviceHistoryRecord(const Rest::Request &, Http::ResponseWriter);

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    std::shared_ptr<HistoryRepository> historyRepository;
    Rest::Router router;
};