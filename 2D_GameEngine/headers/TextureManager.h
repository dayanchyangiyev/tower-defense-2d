#ifndef TextureManager_h
#define TextureManager_h

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* ren);
};

#endif /* TextureManager_h */
