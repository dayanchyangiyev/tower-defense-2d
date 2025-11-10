#ifndef Game_hpp
#define Game_hpp

#include "SDL3/SDL.h"
#include <iostream>
#include <SDL3_image/SDL_image.h>

class Game {

public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() {
        return isRunning;
    }

    friend std::ostream& operator<<(std::ostream& os, const Game& game);

private:
    bool isRunning;
    int count = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;
};

#endif /* Game_hpp */
