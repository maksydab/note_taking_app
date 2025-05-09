#ifndef TASK_DETAILS_H
#define TASK_DETAILS_H

#include "imgui/imgui.h"
#include <string>
#include <vector>

struct Task {
    std::string title;
    std::string text;
    std::string dueDate;
    std::string addedDate;  // Date when task was created
    int reminder; // 0 = Never, 1 = Hourly, 2 = Daily, 3 = Weekly
};


bool ShowTaskDetails(std::vector<Task>& tasks, Task& task);

#endif