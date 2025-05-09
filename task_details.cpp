#include "task_details.h"
#include <algorithm> // Add this for std::find_if
#include <ctime>

bool ShowTaskDetails(std::vector<Task>& tasks, Task& task) {
    ImGui::SetNextWindowSize(ImVec2(450, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Task Details");

    char titleBuffer[128];
    strncpy(titleBuffer, task.title.c_str(), sizeof(titleBuffer) - 1);
    titleBuffer[sizeof(titleBuffer) - 1] = '\0';
    ImGui::InputText("Title", titleBuffer, sizeof(titleBuffer));
    task.title = titleBuffer;

    char textBuffer[512];
    strncpy(textBuffer, task.text.c_str(), sizeof(textBuffer) - 1);
    textBuffer[sizeof(textBuffer) - 1] = '\0';
    ImGui::InputTextMultiline("Description", textBuffer, sizeof(textBuffer));
    task.text = textBuffer;

    static int year = 2025, month = 5, day = 9;
    ImGui::Text("Due Date:");
    ImGui::InputInt("Year", &year);
    ImGui::InputInt("Month", &month);
    ImGui::InputInt("Day", &day);
    task.dueDate = std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);

    bool closePopup = false;

    if (ImGui::Button("Save")) {
        if (task.title.empty()) {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Title cannot be empty!");
        } else {
            auto it = std::find_if(tasks.begin(), tasks.end(), [&](const Task& t) {
                return t.title == task.title;
            });

            if (it != tasks.end()) {
                *it = task;
            } else {
                tasks.push_back(task);
            }

            closePopup = true;
        }
    }

    if (closePopup) ImGui::CloseCurrentPopup();
    ImGui::End();
    return !closePopup;
}
