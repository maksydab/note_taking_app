#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#include <windows.h>
#include <GL/gl.h>
#include <vector>
#include <string>
#include <fstream>
#include "task_details.h"
#include "note_details.h"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

std::vector<Task> tasks;
std::vector<Note> notes;
bool showTaskDetails = false;
bool showNoteDetails = false;
Task currentTask;
Note currentNote;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;
    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void LoadFonts() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("./RobotoCondensed-Italic-VariableFont_wght.ttf", 18.0f);
}

void ApplyModernTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 12.0f;
    style.FrameRounding = 8.0f;
    style.GrabRounding = 8.0f;
    style.ScrollbarRounding = 8.0f;

    ImVec4 bgColor = ImVec4(0.96f, 0.94f, 0.90f, 1.0f);
    ImVec4 darkerBg = ImVec4(0.86f, 0.83f, 0.78f, 1.0f);
    ImVec4 accentColor = ImVec4(0.60f, 0.44f, 0.30f, 1.0f);
    ImVec4 textColor = ImVec4(0.2f, 0.1f, 0.0f, 1.0f);

    style.Colors[ImGuiCol_WindowBg] = bgColor;
    style.Colors[ImGuiCol_ChildBg] = darkerBg;
    style.Colors[ImGuiCol_FrameBg] = darkerBg;
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.8f, 0.7f, 0.6f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.7f, 0.6f, 0.5f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = accentColor;
    style.Colors[ImGuiCol_TitleBgActive] = accentColor;
    style.Colors[ImGuiCol_TitleBgCollapsed] = accentColor;
    style.Colors[ImGuiCol_Button] = accentColor;
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.7f, 0.5f, 0.4f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5f, 0.3f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_Header] = accentColor;
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.7f, 0.5f, 0.4f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.5f, 0.3f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_Text] = textColor;
    style.Colors[ImGuiCol_PopupBg] = darkerBg;
    
}


void SaveData() {
    std::ofstream file("data.txt");
    if (!file.is_open()) return;
    file << "[Tasks]\n";
    for (const auto& task : tasks)
        file << task.title << "\n" << task.text << "\n" << task.dueDate << "\n" << task.addedDate << "\n" << task.reminder << "\n---\n";
    
    file << "[Notes]\n";
    for (const auto& note : notes)
        file << note.title << "\n" << note.text << "\n" << note.addedDate << "\n---\n";

    file.close();
}

void LoadData() {
    tasks.clear();
    notes.clear();
    std::ifstream file("data.txt");
    if (!file.is_open()) return;

    std::string line;
    bool readingTasks = false, readingNotes = false;
    while (std::getline(file, line)) {
        if (line == "[Tasks]") {
            readingTasks = true;
            readingNotes = false;
            continue;
        }
        if (line == "[Notes]") {
            readingTasks = false;
            readingNotes = true;
            continue;
        }

        if (readingTasks) {
            Task task;
            task.title = line;
            std::getline(file, task.text);
            std::getline(file, task.dueDate);
            std::getline(file, task.addedDate);
            file >> task.reminder;
            std::getline(file, line); 
            std::getline(file, line); 
            tasks.push_back(task);
        } else if (readingNotes) {
            Note note;
            note.title = line;
            std::getline(file, note.text);
            std::getline(file, note.addedDate);
            std::getline(file, line); 
            notes.push_back(note);
        }
    }
    file.close();
}

void MainMenu() {
    ImGui::Begin("Notetaking App", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);

    if (ImGui::Button("Save Data")) SaveData();
    ImGui::SameLine();
    if (ImGui::Button("Load Data")) LoadData();
    ImGui::Separator();

    if (ImGui::Button("+")) ImGui::OpenPopup("CreatePopup");
    if (ImGui::BeginPopup("CreatePopup")) {
        if (ImGui::Selectable("Create Task")) {
            showTaskDetails = true;
            currentTask = Task();
        }
        if (ImGui::Selectable("Create Note")) {
            showNoteDetails = true;
            currentNote = Note();
        }
        ImGui::EndPopup();
    }

    ImGui::Text("Tasks & Notes");
    for (auto& task : tasks) {
        ImGui::BeginChild(task.title.c_str(), ImVec2(250, 150), true);
        ImGui::Text("Task: %s", task.title.c_str());
        ImGui::Text("Due: %s", task.dueDate.c_str());
        if (ImGui::Button("Open")) {
            showTaskDetails = true;
            currentTask = task;
        }
        ImGui::EndChild();
    }

    for (auto& note : notes) {
        ImGui::BeginChild(note.title.c_str(), ImVec2(250, 150), true);
        ImGui::Text("Note: %s", note.title.c_str());
        if (ImGui::Button("Open")) {
            showNoteDetails = true;
            currentNote = note;
        }
        ImGui::EndChild();
    }

    ImGui::End();
}

int main() {
    WNDCLASSA wc = { sizeof(WNDCLASSA) };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "NotetakingApp";
    RegisterClassA(&wc);
    HWND hwnd = CreateWindowA(wc.lpszClassName, "Notetaking App", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, wc.hInstance, NULL);
    ShowWindow(hwnd, SW_SHOW);

    HDC hdc = GetDC(hwnd);
    PIXELFORMATDESCRIPTOR pfd = { sizeof(pfd), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32 };
    SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
    HGLRC hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    LoadFonts();
    ApplyModernTheme();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplOpenGL3_Init("#version 130");

    LoadData();

    MSG msg;
    bool running = true;
    while (running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) running = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        MainMenu();
        if (showTaskDetails) showTaskDetails = ShowTaskDetails(tasks, currentTask);
        if (showNoteDetails) showNoteDetails = ShowNoteDetails(notes, currentNote);

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SwapBuffers(hdc);
    }

    SaveData();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    return 0;
}
