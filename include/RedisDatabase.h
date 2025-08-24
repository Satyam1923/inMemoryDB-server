#ifndef REDIS_DATABASE_H
#define REDIS_DATABASE_H

#include <string>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <chrono>

class RedisDatabase{
    public:
    static RedisDatabase& getInstance();

    bool flushAll();

    //key value operations
    void set(const std::string &key,const std::string&value);
    bool get(const std::string &key, std::string&value);
    std::vector<std::string> keys();
    std::string type(const std::string&key);
    bool del(const std::string &key);
    bool expire(const std::string &key, int seconds);
    void purgeExpired();
    bool rename(const std::string &oldkey,const std::string&newkey);

    //list operations
    ssize_t llen(const std::string &key);
    void lpush(const std::string &key, const std::string &value);
    void rpush(const std::string &key, const std::string &value);
    bool lpop(const std::string &key,std::string& value);
    bool rpop(const std::string &key, std::string &value);
    int lrem(const std::string &key,int count,const std::string &value);
    bool lindex(const std::string &key,int index,std::string &value);
    bool lset(const std::string &key, int index, const std::string &value);

    // db operations
    bool dump(const std::string &filename);
    bool load(const std::string& filename);

    private:
    RedisDatabase() = default;
    ~RedisDatabase() = default;
    RedisDatabase(const RedisDatabase&) = delete;
    RedisDatabase& operator=(const RedisDatabase&) = delete;

    std::mutex db_mutex;
    //data types -> 
    std::unordered_map<std::string,std::string> kv_store;
    std::unordered_map<std::string, std::vector<std::string>> list_store;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> hash_store;
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> expiry_map;
};

#endif