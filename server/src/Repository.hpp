#include <sw/redis++/redis++.h>

using namespace sw::redis;

struct Device {
    int id;
    std::string displayText;
    int temperature;
};

class Repository {
public:
    Repository();
    Device getDevice(int);
private:
    Redis redis_;
};
