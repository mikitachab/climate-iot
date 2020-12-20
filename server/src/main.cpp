#include "ClimateEndpoint.hpp"
// #include <sw/redis++/redis++.h>

int main() {
    Address addr(Pistache::Ipv4::any(), Pistache::Port(9080));
    ClimateEndpoint ce(addr);
    ce.init();
    ce.start();
}
