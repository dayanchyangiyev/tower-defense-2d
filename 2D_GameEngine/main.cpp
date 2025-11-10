#include "Game.hpp"

Game *game = nullptr;


int main() {
    
    const int FPS = 30;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();
    game->init("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
    
    while (game->running()){

        frameStart = SDL_GetTicks(); //how many milliseconds has passed since the starting of the game
        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart; //how much time it took to do all the work:
                                                 // handling, updating and rendering
        if(frameDelay > frameTime)
        { //we need to delay 
            SDL_Delay(frameDelay - frameTime);
        }                                         
    }

    game->clean();

    return 0;
}
