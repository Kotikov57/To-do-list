#include "../include/crow.h"
#include "../include/task_manager.h"
#include "../include/server.h"
#include "../include/database.h"
#include <sqlite3.h>

void Server::initRoutes() {
    crow::SimpleApp app;

    Database::init();

    CROW_ROUTE(app, "/tasks").methods(crow::HTTPMethod::GET)([](const crow::request&){
        std::vector<Task> tasks = TaskManager::getAllTasks();
        crow::json::wvalue jsonResponse;
        for (size_t i = 0; i < tasks.size(); ++i) {
            jsonResponse["tasks"][i]["id"] = tasks[i].id;
            jsonResponse["tasks"][i]["description"] = tasks[i].description;
            jsonResponse["tasks"][i]["priority"] = tasks[i].priority;
            jsonResponse["tasks"][i]["status"] = tasks[i].status ? "Completed" : "Pending";
        }
        // Возвращаем JSON-ответ
        return crow::response{jsonResponse};
    });
    CROW_ROUTE(app, "/tasks").methods("POST"_method)([](const crow::request& req) {
        auto task = crow::json::load(req.body);
        if (!task.has("description") || !task.has("priority")) {
            return crow::response(400, "Missing required fields");
        }
        std::string description = task["description"].s();
        int priority = task["priority"].i();  
        const char* sql = "INSERT INTO tasks (description, priority, status) VALUES (?, ?, ?);";

        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
            return crow::response(555);
        }
        if (sqlite3_bind_text(stmt, 1, description.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
            std::cerr << "Error binding description: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return crow::response(555);
        }
        if (sqlite3_bind_int(stmt, 2, priority) != SQLITE_OK) {
            std::cerr << "Error binding priority: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return crow::response(555);
        }
        if (sqlite3_bind_int(stmt, 3, 0) != SQLITE_OK) { // false для статуса
            std::cerr << "Error binding status: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return crow::response(555);
        }
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error executing statement: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cout << "Task inserted successfully!" << std::endl;
        }

        sqlite3_finalize(stmt);
        return crow::response(201);
    });

    CROW_ROUTE(app, "/tasks/<int>").methods("DELETE"_method)([](const crow::request&, int id) {
        TaskManager::deleteTask(id);
        return crow::response(204);
    });
    app.port(8080).multithreaded().run();
}