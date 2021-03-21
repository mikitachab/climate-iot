#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "SQLite.hpp"
#include "ClimateRepository.hpp"


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
    int id;
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

struct GetDeviceHistoryQuery
{
    DeviceHistory deviceHistory;
    int deviceId;
    GetDeviceHistoryQuery(int id) : deviceId(id) {}
    static int callback(void *obj, int argc, char **argv, char **azColName)
    {
        GetDeviceHistoryQuery *self = static_cast<GetDeviceHistoryQuery *>(obj);
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

struct GetDeviceByNameQuery
{
    Device device;
    std::string name;
    bool found{false};
    GetDeviceByNameQuery(std::string name) : name(name) {}
    static int callback(void *obj, int argc, char **argv, char **azColName)
    {
        GetDeviceByNameQuery *self = static_cast<GetDeviceByNameQuery *>(obj);
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

class SQLiteHistoryRepository : public IClimateRepository
{
public:
    SQLiteHistoryRepository(std::shared_ptr<SQLite> db) : db(db) {
        init();
    }
    void init()
    {
        // TODO fix error handling
        CreateHistoryTablesQuery chtq;
        db->execute(chtq.query());
    }

    Result<Device> addDevice(std::string name)
    {
        AddDeviceQuery adq{name};
        if (db->execute(adq.query()) != SQLITE_OK){
            return Result<Device>::fail(db->errorMsg);
        }
        int id = db->lastInsertedId();
        return Result<Device>::success(Device{id, name});
    }

    Result<> addHistoryRecord(int deviceId, float temperature, std::string datetime)
    {
        AddHistoryRecordQuery ahr{deviceId, temperature, datetime};
        if(db->execute(ahr.query()) != SQLITE_OK){
            return Result<>::fail(db->errorMsg);
        };
        return Result<>::success();
    }

    std::string error()
    {
        if (db->errorMsg.size() == 0)
        {
            return "unknown";
        }
        return db->errorMsg;
    }
    Result<std::vector<Device>> getAllDevices()
    {
        GetAllDevices gad;
        if(db->execute(gad.query()) != SQLITE_OK){
            return Result<std::vector<Device>>::fail(db->errorMsg);
        };
        return Result<std::vector<Device>>::success(gad.devices);
    }

    Result<DeviceHistory> getDeviceHistory(int deviceId)
    {
        GetDeviceHistoryQuery gdh(deviceId);
        if(db->execute(gdh.query()) != SQLITE_OK){
            return Result<DeviceHistory>::fail(db->errorMsg);
        };
        return Result<DeviceHistory>::success(gdh.deviceHistory);
    }

    Result<int> getDeviceIdByName(std::string name)
    {
        GetDeviceByNameQuery gdbn(name);
        db->execute(gdbn.query());
        if (gdbn.found){
            return Result<int>::success(gdbn.device.id);
        }
        return Result<int>::fail("Device not found");
    }

    Result<DeviceHistory> getDeviceHistoryByDeviceName(std::string name)
    {
        auto result = getDeviceIdByName(name);
        if (!result.ok)
        {
            return Result<DeviceHistory>::fail(result.errorMsg.value());
        }
        return getDeviceHistory(result.data.value());
    }

private:
    std::shared_ptr<SQLite> db;
};
