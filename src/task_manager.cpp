#include "../include/task_manager.h"
#include "../include/database.h"

std::vector<std::string> TaskManager::getAllTasks() {
    return Database::query("SELECT * FROM tasks");
}

void TaskManager::addTask(const std::string& description, int priority) {
    Database::execute("INSERT INTO tasks (description, priority, status) VALUES (?, ?, ?)",
                     description, priority, false);
}

void TaskManager::deleteTask(int id) {
    Database::execute("DELETE FROM tasks WHERE id = ?", id);
}