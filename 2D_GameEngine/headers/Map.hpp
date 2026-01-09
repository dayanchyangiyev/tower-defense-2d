#ifndef Map_hpp
#define Map_hpp

#include "Game.hpp"

class Map {
public:
    Map(SDL_Renderer* ren);
    ~Map();

    void LoadMap(int arr[20][25]); // 800x640 / 32 = 25x20
    void DrawMap();

private:
    SDL_FRect src, dest;
    SDL_Texture* dirt;
    SDL_Texture* grass;
    SDL_Texture* water;
    
    int map[20][25];
    SDL_Renderer* renderer;
};

#endif /* Map_hpp */
