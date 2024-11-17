#include "../include/crow.h"
#include "../include/task_manager.h"
#include "../include/server.h"
#include "../include/database.h"

void Server::initRoutes() {
    crow::SimpleApp app;

    Database::init();

    CROW_ROUTE(app, "/tasks").methods(crow::HTTPMethod::GET)([](const crow::request&){
        std::vector<std::string> a = TaskManager::getAllTasks();
        return crow::response(200);
    });

    CROW_ROUTE(app, "/tasks").methods("POST"_method)([](const crow::request& req) {
        auto task = crow::json::load(req.body);
        if (!task) {
        return crow::response(400, "Invalid JSON");
        }

        if (!task.has("description") || !task.has("priority")) {
            return crow::response(400, "Missing required fields");
        }
        std::string description = task["description"].s();
        if (description.empty()) {
        CROW_LOG_ERROR << "Description is empty!";
        } 
        int priority = task["priority"].i();  
        CROW_LOG_INFO << "Description: " << description;
        CROW_LOG_INFO << "Priority: " << priority;   
        TaskManager::addTask(description, priority);
        return crow::response(201);
    });

    CROW_ROUTE(app, "/tasks/<int>").methods("DELETE"_method)([](const crow::request&, int id) {
        TaskManager::deleteTask(id);
        return crow::response(204);
    });
    app.port(8080).multithreaded().run();
}