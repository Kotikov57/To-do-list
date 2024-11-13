#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

class Database {
public:
    static void execute(const std::string& query, ...);
    static std::vector<std::string> query(const std::string& query);
    static void init();
};

#endif