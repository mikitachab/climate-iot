#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "SQLite.hpp"

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

struct CreateHistoryTablesQuery
{
    static int callback(void *obj, int argc, char **argv, char **azColName)
    {
        return 0;
    }
    std::string sql()
    {
        std::string sqlStr = "CREATE TABLE IF NOT EXISTS device("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                             "name TEXT UNIQUE NOT NULL);";
        sqlStr += "CREATE TABLE IF NOT EXISTS history("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "device_id INEGER NOT NULL,"
                  "temperature REAL NOT NULL,"
                  "datetime DATETIME NOT NULL,"
                  "FOREIGN KEY(device_id) REFERENCES device(id));";
        return sqlStr;
    }

    SQLite::Query query()
    {
        return SQLite::Query{sql(), callback, nullptr};
    }
};

struct AddDeviceQuery
{
    std::string deviceName;
    static int callback(void *obj, int argc, char **argv, char **azColName)
    {
        return 0;
    }

    std::string sql()
    {
        return "INSERT INTO device (name) VALUES(\"" + deviceName + "\");";
    }

    SQLite::Query query()
    {
        return SQLite::Query{sql(), callback, nullptr};
    }
};

struct GetAllDevices
{
    std::vector<Device> devices;
    static int callback(void *obj, int argc, char **argv, char **azColName)
    {
        GetAllDevices *self = static_cast<GetAllDevices *>(obj);
        Device device;
        const char *id = argv[0] ? argv[0] : "-1";
        device.id = std::atoi(id);
        const char *name = argv[1] ? argv[1] : "NULL";
        device.name = std::string(name);
        self->devices.push_back(device);
        return 0;
    }

    std::string sql()
    {
        return "SELECT id, name FROM device;";
    }

    SQLite::Query query()
    {
        return SQLite::Query{sql(), callback, static_cast<void *>(this)};
    }
};

struct AddHistoryRecordQuery
{
    int deviceId;
    float temperature;
    std::string datetime;
    static int callback(void *obj, int argc, char **argv, char **azColName)
    {
        return 0;
    }

    std::string sql()
    {
        std::string sqlStr = "INSERT INTO history (device_id, temperature, datetime) VALUES(";
        sqlStr += std::to_string(deviceId) + ", " + std::to_string(temperature) + ", ";
        sqlStr += "\"" + datetime + "\");";
        return sqlStr;
    }

    SQLite::Query query()
    {
        return SQLite::Query{sql(), callback, nullptr};
    }
};

struct GetDeviceHistory
{
    DeviceHistory deviceHistory;
    int deviceId;
    GetDeviceHistory(int id) : deviceId(id) {}
    static int callback(void *obj, int argc, char **argv, char **azColName)
    {
        GetDeviceHistory *self = static_cast<GetDeviceHistory *>(obj);
        HistoryRecord r;
        const char *temperature = argv[0] ? argv[0] : "-1";
        r.temperature = std::atof(temperature);
        const char *datetime = argv[1] ? argv[1] : "NULL";
        r.datetime = std::string(datetime);
        self->deviceHistory.data.push_back(r);
        return 0;
    }

    std::string sql()
    {
        return "SELECT temperature, datetime FROM history WHERE device_id=" + std::to_string(deviceId) + ";";
    }

    SQLite::Query query()
    {
        return SQLite::Query{sql(), callback, static_cast<void *>(this)};
    }
};

struct GetDeviceByName
{
    Device device;
    std::string name;
    bool found{false};
    GetDeviceByName(std::string name) : name(name) {}
    static int callback(void *obj, int argc, char **argv, char **azColName)
    {
        GetDeviceByName *self = static_cast<GetDeviceByName *>(obj);
        const char *id = argv[0] ? argv[0] : "-1";
        self->device.name = self->name;
        self->device.id = std::stoi(id);
        self->found = true;
        return 0;
    }

    std::string sql()
    {
        return "SELECT id FROM device WHERE name= \"" + name + "\";";
    }

    SQLite::Query query()
    {
        return SQLite::Query{sql(), callback, static_cast<void *>(this)};
    }
};

class HistoryRepository
{
public:
    HistoryRepository(std::shared_ptr<SQLite> db) : db(db) {}
    int init()
    {
        CreateHistoryTablesQuery chtq;
        return db->execute(chtq.query());
    }

    int addDevice(std::string deviceName)
    {
        AddDeviceQuery adq{deviceName};
        return db->execute(adq.query());
    }

    int addHistoryRecord(int deviceId, float temperature, std::string datetime)
    {
        AddHistoryRecordQuery ahr{deviceId, temperature, datetime};
        return db->execute(ahr.query());
    }

    std::string error()
    {
        if (db->errorMsg.size() == 0)
        {
            return "unknown";
        }
        return db->errorMsg;
    }
    std::vector<Device> getAllDevices()
    {
        GetAllDevices gad;
        // TODO how to check for error here?
        db->execute(gad.query());
        return gad.devices;
    }

    DeviceHistory getDeviceHistory(int deviceId)
    {
        GetDeviceHistory gdh(deviceId);
        // TODO how to check for error here?
        db->execute(gdh.query());
        return gdh.deviceHistory;
    }

    int getDeviceIdByName(std::string name)
    {
        GetDeviceByName gdbn(name);
        db->execute(gdbn.query());
        return gdbn.found ? gdbn.device.id : -1;
    }

    DeviceHistory getDeviceHistoryByDeviceName(std::string name)
    {
        int deviceId = getDeviceIdByName(name);
        if (deviceId == -1)
        {
            return DeviceHistory{false, std::vector<HistoryRecord>(0)};
        }
        return getDeviceHistory(deviceId);
    }

private:
    std::shared_ptr<SQLite> db;
};
