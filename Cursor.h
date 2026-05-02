#ifndef ROGUEFUN_CURSOR_H
#define ROGUEFUN_CURSOR_H
#include <SDL3/SDL_events.h>

#include "LTexture.h"


class Cursor {
public:
    Cursor();
    ~Cursor();
    Cursor( const Cursor& ) = delete;
    Cursor( Cursor&& ) = delete;
    Cursor& operator=( const Cursor& ) = delete;
    Cursor& operator=( Cursor&& ) = delete;

    // Handle mouse event.
    bool handleEvent( SDL_Event* e );

    // Render cursor.
    void render();
private:
    // Mouse position.
    SDL_FPoint position;

    // Cursor icon.
    LTexture* texture;
};


#endif //ROGUEFUN_CURSOR_H
