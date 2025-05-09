#include "note_details.h"
#include <algorithm> // Add this for std::find_if

bool ShowNoteDetails(std::vector<Note>& notes, Note& note) {
    ImGui::SetNextWindowSize(ImVec2(450, 300), ImGuiCond_FirstUseEver);
    ImGui::Begin("Note Details");

    char titleBuffer[128];
    strncpy(titleBuffer, note.title.c_str(), sizeof(titleBuffer) - 1);
    titleBuffer[sizeof(titleBuffer) - 1] = '\0';
    ImGui::InputText("Title", titleBuffer, sizeof(titleBuffer));
    note.title = titleBuffer;

    char textBuffer[512];
    strncpy(textBuffer, note.text.c_str(), sizeof(textBuffer) - 1);
    textBuffer[sizeof(textBuffer) - 1] = '\0';
    ImGui::InputTextMultiline("Content", textBuffer, sizeof(textBuffer));
    note.text = textBuffer;

    bool closePopup = false;

    if (ImGui::Button("Save")) {
        if (note.title.empty()) {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Title cannot be empty!");
        } else {
            auto it = std::find_if(notes.begin(), notes.end(), [&](const Note& n) {
                return n.title == note.title;
            });

            if (it != notes.end()) {
                *it = note;
            } else {
                notes.push_back(note);
            }

            closePopup = true;
        }
    }

    if (closePopup) ImGui::CloseCurrentPopup();
    ImGui::End();
    return !closePopup;
}
