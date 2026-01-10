#include "Game.hpp"
#include "Logger.hpp"
#include "GameObject.h"

Game *game = nullptr;

int main() {
    Logger::getInstance().init("game_log.txt");
    
    // const int FPS = 30; // Defined in Game loop usually or managed here
    const int FPS = 30;
    const int frameDelay = 1000 / FPS;



    try {
        game = new Game();
        game->init("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

#ifdef GITHUB_ACTIONS
        const Uint32 maxRuntimeMs = 2000;
        const Uint32 startTicks = SDL_GetTicks();
#endif

        while (game->running()){
            Uint32 frameStart = SDL_GetTicks(); 
            game->handleEvents();
            game->update();
            game->render();

            int frameTime = SDL_GetTicks() - frameStart; 
            if(frameDelay > frameTime)
            { 
                SDL_Delay(frameDelay - frameTime);
            }                                         

#ifdef GITHUB_ACTIONS
            if (SDL_GetTicks() - startTicks > maxRuntimeMs) {
                Logger::getInstance().log("CI time limit reached, exiting.");
                break;
            }
#endif
        }
    } catch (const GameException& e) {
        std::string message("CRITICAL EXCEPTION: ");
        message += e.what();
        Logger::getInstance().log(message);
        std::cerr << "Game Crash: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::string message("STD EXCEPTION: ");
        message += e.what();
        Logger::getInstance().log(message);
        return -1;
    } catch (...) {
        Logger::getInstance().log("UNKNOWN EXCEPTION");
        return -1;
    }

    game->clean();

    return 0;
}
