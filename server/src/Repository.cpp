#include "Repository.hpp"

Repository::Repository(): redis_(Redis("tcp://127.0.0.1:6379")) {}

Device Repository::getDevice(int){
    return Device{};
};
