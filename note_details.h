#ifndef NOTE_DETAILS_H
#define NOTE_DETAILS_H

#include "imgui/imgui.h"
#include <vector>
#include <string>

struct Note {
    std::string title;
    std::string text;
    std::string addedDate;  // Date when note was created
};


bool ShowNoteDetails(std::vector<Note>& notes, Note& note);

#endif
