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

    enum GameState {
        MENU,
        PLAYING,
        EXIT
    };

private:
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
    
    GameState gameState;
    
    // Menu Assets
    SDL_Texture* menuBg;
    SDL_Texture* btnStart;
    SDL_Texture* btnQuit;
    SDL_FRect startRect, quitRect;
    
    class Level* level;
};

#endif /* Game_hpp */
