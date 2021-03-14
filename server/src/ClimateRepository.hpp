#pragma once
#include <vector>

#include "Result.hpp"

struct Device
{
    int id;
    std::string name;
};

struct HistoryRecord
{
    // int deviceId;
    float temperature;
    std::string datetime;
};

struct DeviceHistory
{
    bool deviceExists{true};
    std::vector<HistoryRecord> data;
};

class IClimateRepository {
public:
    virtual int addDevice(std::string deviceName) = 0;
    virtual int addHistoryRecord(int deviceId, float temperature, std::string datetime) = 0;
    virtual std::vector<Device> getAllDevices() = 0;
    virtual DeviceHistory getDeviceHistory(int deviceId) = 0;
    virtual int getDeviceIdByName(std::string name) = 0;
    virtual DeviceHistory getDeviceHistoryByDeviceName(std::string name) = 0;
};