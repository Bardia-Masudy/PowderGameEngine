#include "Cursor.h"

Cursor::Cursor() {
}

Cursor::~Cursor() {
}

bool Cursor::handleEvent(SDL_Event *e) {
    // If mouse click happened
    if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        // Get mouse position.
        // float x = -1.f, y = -1.f;
        // SDL_GetMouseState(&x, &y);

        return true;
    }
    return false;
}

void Cursor::render() {

}
