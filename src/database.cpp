#include "../include/database.h"
#include <sqlite3.h>
#include <stdexcept>
#include <iostream>

sqlite3* db = nullptr;

void Database::init() {
    if (sqlite3_open("tasks.db", &db)) {
        std::cerr << "Ошибка открытия базы данных: " << sqlite3_errmsg(db) << std::endl;
        throw std::runtime_error("Не удалось открыть базу данных");
    }
    const char* sqlCreateTable = R"(
        CREATE TABLE IF NOT EXISTS tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            description TEXT NOT NULL,
            priority INTEGER NOT NULL,
            status BOOLEAN
        );
    )";
    char* errorMessage = nullptr;
    if (sqlite3_exec(db, sqlCreateTable, 0, 0, &errorMessage) != SQLITE_OK) {
        std::cerr << "Ошибка создания таблицы: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        throw std::runtime_error("Не удалось создать таблицу");
    }
}

std::vector<std::string> Database::query(const std::string& query) {
    
    sqlite3_stmt* stmt;
    std::vector<std::string> results;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        throw std::runtime_error("Ошибка подготовки SQL запроса");
    }
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        std::string taskDescription = reinterpret_cast<const char*>(sqlite3_column_text(stmt,1));
        results.push_back(taskDescription);
    }
    sqlite3_finalize(stmt);
    return results;
}

void Database::execute(const std::string& query, ...) {
    char* errorMessage = nullptr;
    if (sqlite3_exec(db, query.c_str(), 0, 0, &errorMessage) != SQLITE_OK) {
        std::cerr << "Ошибка выполнения запроса: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        throw std::runtime_error("Ошибка выполнения запроса");
    }
}