#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <string>
#include <vector>
#include "database.h"

class TaskManager {
public:
    static std::vector<Task> getAllTasks();
    static void addTask(const std::string& description, int priority);
    static void deleteTask(int id);
};

#endif