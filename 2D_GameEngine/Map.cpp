#include "Map.hpp"
#include "GameObject.h"
#include "Logger.hpp"

Map::Map(SDL_Renderer* ren) {
    renderer = ren;
    
    try {
        grass = TextureManager::LoadTexture("assets/map_tile.bmp", ren);
        dirt = TextureManager::LoadTexture("assets/path_tile.bmp", ren);
    } catch (const ResourceError& e) {
        // Critical map textures
        // Maybe throw "MapLoadError" derived from GameException?
        // For now rely on propagation or log.
        // Let's propagate to main if critical.
        throw; 
    }
    
    try {
        water = TextureManager::LoadTexture("assets/water.png", ren);
    } catch (const ResourceError& e) {
        // Optional texture
        Logger::log("Warning: Water texture missing. Proceeding without it.");
        water = nullptr;
    }
    
    src.x = src.y = 0;
    src.w = dest.w = 32;
    src.h = dest.h = 32;
    
    dest.x = dest.y = 0;
}

Map::~Map() {
    SDL_DestroyTexture(grass);
    SDL_DestroyTexture(dirt);
}

void Map::LoadMap(int arr[20][25]) {
    for (int row = 0; row < 20; row++) {
        for (int col = 0; col < 25; col++) {
            map[row][col] = arr[row][col];
        }
    }
}

void Map::DrawMap() {
    int type = 0;
    
    for (int row = 0; row < 20; row++) {
        for (int col = 0; col < 25; col++) {
            type = map[row][col];
            
            dest.x = col * 32;
            dest.y = row * 32;
            
            switch (type) {
                case 0:
                    SDL_RenderTexture(renderer, grass, &src, &dest);
                    break;
                case 1:
                    SDL_RenderTexture(renderer, dirt, &src, &dest);
                    break;
                default:
                    SDL_RenderTexture(renderer, grass, &src, &dest);
                    break;
            }
        }
    }
}
