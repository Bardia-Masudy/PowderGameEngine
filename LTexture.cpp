#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>

#include "LTexture.h"

#include "WindowManager.h"


LTexture::LTexture(): mTexture{ nullptr }, mWidth{ 0 }, mHeight{ 0 } {}

LTexture::~LTexture() { destroy(); }

bool LTexture::loadFromFile( std::string path ) {
    // Clean up texture if it already exists
    destroy();

    // Load surface
    if (SDL_Surface* loadedSurface = IMG_Load(path.c_str()); loadedSurface == nullptr ) {
        SDL_Log( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError() );
    } else {
        // Create texture from surface
        if ( mTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface); mTexture == nullptr ) {
            SDL_Log("Unable to create texture from loaded picture! SDL Error: %s\n", SDL_GetError() );
        } else {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Clean up loaded surface
        SDL_DestroySurface( loadedSurface );
    }

    // Return success if texture loaded
    return mTexture != nullptr;
}

void LTexture::destroy() {
    SDL_DestroyTexture( mTexture );
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

void LTexture::render( float x, float y ) const {
    SDL_FRect dstRect{ x, y, static_cast<float>( mWidth ), static_cast<float>( mHeight ) };

    SDL_RenderTexture( gRenderer, mTexture, nullptr, &dstRect);
}

int LTexture::getWidth() const
{
    return mWidth;
}

int LTexture::getHeight() const
{
    return mHeight;
}

bool LTexture::isLoaded() const {
    return mTexture != nullptr;
}
