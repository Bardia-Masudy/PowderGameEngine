#ifndef ROGUEFUN_LTEXTURE_H
#define ROGUEFUN_LTEXTURE_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>

class LTexture {
    public:
    // Initializes texture variables.
    LTexture();

    // Cleans up texture variables.
    ~LTexture();

    //Remove copy constructor
    LTexture( const LTexture& ) = delete;

    //Remove copy assignment
    LTexture& operator=( const LTexture& ) = delete;

    //Remove move constructor
    LTexture( LTexture&& ) = delete;

    //Remove move assignment
    LTexture& operator=( LTexture&& ) = delete;

    // Loads texture from disk.
    bool loadFromFile(  std::string path );

    // Cleans up texture.
    void destroy();

    // Draws texture.
    void render( float x, float y ) const;

    // Gets texture attributes.
    int getWidth() const;
    int getHeight() const;
    bool isLoaded() const;

private:
    // Contains texture data.
    SDL_Texture* mTexture;

    // Texture dimensions.
    int mWidth;
    int mHeight;

};


#endif //ROGUEFUN_LTEXTURE_H
