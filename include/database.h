#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <sqlite3.h>

extern sqlite3* db;

struct Task {
    int id;
    std::string description;
    int priority;
    bool status;
};

class Database {
public:
    static void execute(const std::string& query, ...);
    static std::vector<Task> query(const std::string& query);
    static void init();
};



#endif