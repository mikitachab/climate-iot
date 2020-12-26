#include <iostream>
#include "ClimateEndpoint.hpp"

ClimateEndpoint::ClimateEndpoint(Address addr) : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
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

void ClimateEndpoint::getDeviceHistory(const Rest::Request &request, Http::ResponseWriter response)
{
    response.send(Http::Code::Ok, "get device history");
}

void ClimateEndpoint::setDeviceDisplay(const Rest::Request &request, Http::ResponseWriter response)
{
    std::cout << request.body() << std::endl;
    response.send(Http::Code::Ok, "set device display");
}
