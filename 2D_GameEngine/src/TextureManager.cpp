#include "TextureManager.h"
#include "GameObject.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName, SDL_Renderer* ren) {
    if (!ren) throw InitializationError("Renderer is null in LoadTexture");
    
    SDL_Surface* tempSurface = IMG_Load(fileName);
    if (!tempSurface) {
        std::string err = "Failed to load texture: ";
        err += fileName;
        throw ResourceError(err);
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSurface);
    SDL_DestroySurface(tempSurface);
    
    if (!tex) {
         throw ResourceError("Failed to convert surface to texture for: " + std::string(fileName));
    }
    return tex;
}
