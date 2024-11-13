#include "../include/crow.h"
#include "../include/task_manager.h"
#include "../include/server.h"

void Server::initRoutes() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/tasks").methods(crow::HTTPMethod::GET)([](const crow::request&){
        std::vector<std::string> a = TaskManager::getAllTasks();
        return crow::response(200);
    });
    CROW_ROUTE(app, "/tasks").methods("POST"_method)([](const crow::request& req) {
        auto task = crow::json::load(req.body);
        TaskManager::addTask(task["description"].s(), task["priority"].i());
        return crow::response(201);
    });
    CROW_ROUTE(app, "/tasks/<int>").methods("DELETE"_method)([](const crow::request&, int id) {
        TaskManager::deleteTask(id);
        return crow::response(204);
    });
    app.port(8080).multithreaded().run();
}