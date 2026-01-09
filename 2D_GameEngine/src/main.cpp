#include "Game.hpp"
#include "Logger.hpp"
#include "GameObject.h"

Game *game = nullptr;

int main() {
    Logger::init("game_log.txt");
    
    // const int FPS = 30; // Defined in Game loop usually or managed here
    const int FPS = 30;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    try {
        game = new Game();
        game->init("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
        
        while (game->running()){
            frameStart = SDL_GetTicks(); 
            game->handleEvents();
            game->update();
            game->render();

            frameTime = SDL_GetTicks() - frameStart; 
            if(frameDelay > frameTime)
            { 
                SDL_Delay(frameDelay - frameTime);
            }                                         
        }
    } catch (const GameException& e) {
        Logger::log(std::string("CRITICAL EXCEPTION: ") + e.what());
        std::cerr << "Game Crash: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        Logger::log(std::string("STD EXCEPTION: ") + e.what());
        return -1;
    } catch (...) {
        Logger::log("UNKNOWN EXCEPTION");
        return -1;
    }

    game->clean();

    return 0;
}
