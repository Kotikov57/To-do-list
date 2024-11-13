#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <string>
#include <vector>

class TaskManager {
public:
    static std::vector<std::string> getAllTasks();
    static void addTask(const std::string& description, int priority);
    static void deleteTask(int id);
};

#endif