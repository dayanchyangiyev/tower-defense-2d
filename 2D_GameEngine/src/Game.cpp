#include "Game.hpp"
#include "GameObject.h"
#include "TextureManager.h"
#include "Level.hpp"
#include "Logger.hpp"
#include <sstream>

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), gameState(MENU), menuBg(nullptr), btnStart(nullptr), btnQuit(nullptr), level(nullptr)
{}

Game::~Game()
{
    delete level;
}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if(fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if(SDL_Init(SDL_INIT_VIDEO))
    {
        Logger::log("Subsystem Initialised!...");

        window = SDL_CreateWindow(title, width, height, static_cast<SDL_WindowFlags>(flags)); 
        if(window)
        {
            Logger::log("window created!");
            SDL_SetWindowPosition(window, xpos, ypos);
        }

        renderer = SDL_CreateRenderer(window, NULL);
        if(renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            Logger::log("Renderer created!");
        }
        isRunning = true;

    } else {
        isRunning = false;
    }
    
    // Load Menu Assets
    menuBg = TextureManager::LoadTexture("assets/menu_bg.bmp", renderer);
    btnStart = TextureManager::LoadTexture("assets/btn_start.bmp", renderer);
    btnQuit = TextureManager::LoadTexture("assets/btn_quit.bmp", renderer);
    
    startRect = {300.0f, 200.0f, 200.0f, 64.0f};
    quitRect = {300.0f, 300.0f, 200.0f, 64.0f};
    
    // Init Level
    level = new Level(renderer, 1);
    
    // Simple Map 
    int mapArr[20][25];
    for (int r = 0; r < 20; r++) {
         for (int c = 0; c < 25; c++) {
             mapArr[r][c] = 0; // Grass
         }
    }
    for(int c=0; c<25; c++) mapArr[10][c] = 1; 

    level->loadMap(mapArr);
    
    
    gameState = MENU;
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event); 
    switch (event.type) {
        case SDL_EVENT_QUIT:
            isRunning = false;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (gameState == MENU) {
                float mx = event.button.x;
                float my = event.button.y;
                
                // Check Start
                if (mx >= startRect.x && mx <= startRect.x + startRect.w &&
                    my >= startRect.y && my <= startRect.y + startRect.h) {
                    gameState = PLAYING;
                    Logger::log("Game Started!");
                }
                
                // Check Quit
                if (mx >= quitRect.x && mx <= quitRect.x + quitRect.w &&
                    my >= quitRect.y && my <= quitRect.y + quitRect.h) {
                    isRunning = false;
                }
            } else if (gameState == PLAYING) {
                 
            }
            break;
        case SDL_EVENT_KEY_DOWN:
            if (gameState == PLAYING) {
                if (event.key.key == SDLK_ESCAPE) {
                    gameState = MENU;
                } else {
                    // Pass other keys to Level
                    if (level) level->handleInput(event.key.key);
                }
            }
            break;
        default:
            break;
    }

}
void Game::update()
{
    if (gameState == PLAYING && level) {
         level->update();
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    
    if (gameState == MENU) {
        SDL_RenderTexture(renderer, menuBg, nullptr, nullptr);
        SDL_RenderTexture(renderer, btnStart, nullptr, &startRect);
        SDL_RenderTexture(renderer, btnQuit, nullptr, &quitRect);
    } else if (gameState == PLAYING) {
        if (level) level->render();
    }
    
    SDL_RenderPresent((renderer));
}

void Game::clean()
{
    SDL_DestroyTexture(menuBg);
    SDL_DestroyTexture(btnStart);
    SDL_DestroyTexture(btnQuit);
    
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    window = nullptr;
    renderer = nullptr;
    isRunning = false;
    SDL_Quit();
    Logger::log("Game cleaned");

}

std::ostream& operator<<(std::ostream& os, const Game& game)
{
    os << "Game [Running: " << game.isRunning << ", State: " << game.gameState << "]";
    return os;
}
