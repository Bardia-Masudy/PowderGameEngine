#ifndef ROGUEFUN_ROGUEFUN_H
#define ROGUEFUN_ROGUEFUN_H

#include <SDL3/SDL.h>

/* Constants */
// Screen Constants
constexpr int SCREEN_WIDTH{640};
constexpr int SCREEN_HEIGHT{480};

/* Functions */
// Starts up SDL, and creates the window.
bool init();

// Frees media and shuts down SDL
void closeSDL();

/* Global Vars */
// The window being rendered to.
extern SDL_Window *gWindow;

// The game window renderer.
extern SDL_Renderer *gRenderer;

#endif //ROGUEFUN_ROGUEFUN_H
