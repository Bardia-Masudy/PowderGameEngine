#ifndef ROGUEFUN_ROGUEFUN_H
#define ROGUEFUN_ROGUEFUN_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>


/* Constants */
// Screen Constants
constexpr int ScreenWidth{ 640 };
constexpr int ScreenHeight{ 480 };

/* Functions */
// Starts up SDL, and creates the window.
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void closeSDL();

/* Global Vars */
// The window being rendered to.
inline SDL_Window* gWindow{ nullptr };

// The surface contained by the game window.
inline SDL_Surface* gSurface{ nullptr };

// The image to be loaded onto the screen.
inline SDL_Surface* gHelloWorld{ nullptr };

#endif //ROGUEFUN_ROGUEFUN_H
