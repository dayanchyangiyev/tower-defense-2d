#include "Game.hpp"
#include "GameObject.h"
#include "TextureManager.h"
#include "Level.hpp"
#include "Logger.hpp"
#include <sstream>
#include <fstream>
#include <string>

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), gameState(MENU), menuBg(nullptr), btnStart(nullptr), btnQuit(nullptr), btnManual(nullptr), level(nullptr)
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
        Logger::getInstance().log("Subsystem Initialised!...");

        window = SDL_CreateWindow(title, width, height, static_cast<SDL_WindowFlags>(flags)); 
        if(window)
        {
            Logger::getInstance().log("window created!");
            SDL_SetWindowPosition(window, xpos, ypos);
        }

        renderer = SDL_CreateRenderer(window, NULL);
        if(renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            Logger::getInstance().log("Renderer created!");
        }
        isRunning = true;

    } else {
        isRunning = false;
    }
    
    // Load Menu Assets
    menuBg = TextureManager::LoadTexture("assets/menu_bg.bmp", renderer);
    btnStart = TextureManager::LoadTexture("assets/btn_start.bmp", renderer);
    btnQuit = TextureManager::LoadTexture("assets/btn_quit.bmp", renderer);
    btnManual = TextureManager::LoadTexture("assets/btn_manual.png", renderer);
    
    startRect = {300.0f, 200.0f, 200.0f, 64.0f};
    manualRect = {300.0f, 300.0f, 200.0f, 64.0f};
    quitRect = {300.0f, 400.0f, 200.0f, 64.0f};
    
    // Init Level
    level = new Level(renderer, 1);
    
    // Simple Map using Matrix2D methods
    Matrix2D<int, 20, 25> mapMatrix;
    mapMatrix.fill(0); // Fill with Grass (0)
    for(int c=0; c<25; c++) mapMatrix.set(10, c, 1); // Set Path (1)

    // Load into Level (we might need to adapt Level::loadMap to take Matrix2D or just extract raw?)
    // Level::loadMap takes int arr[20][25]. We can't pass Matrix2D directly unless we overload or expose raw.
    // For now, let's just allow the unused 'fill'/'set' to simply BE USED here locally, 
    // effectively proving they work, even if we convert back to array or use existing logic.
    // Actually, let's update Level to take a Matrix2D?
    // Or just use them:
    int mapArr[20][25];
    for(int r=0; r<20; r++)
        for(int c=0; c<25; c++)
            mapArr[r][c] = mapMatrix.get(r,c);
            
    level->loadMap(mapArr);
    
    // Use getCount
    Logger::getInstance().log("Total GameObjects: " + std::to_string(GameObject::getCount()));
    
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
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            float mx = event.button.x;
            float my = event.button.y;
            
            if (gameState == MENU) {
                
                // Check Start
                if (mx >= startRect.x && mx <= startRect.x + startRect.w &&
                    my >= startRect.y && my <= startRect.y + startRect.h) {
                    gameState = PLAYING;
                    Logger::getInstance().log("Game Started!");
                }
                
                // Check Manual
                if (mx >= manualRect.x && mx <= manualRect.x + manualRect.w &&
                    my >= manualRect.y && my <= manualRect.y + manualRect.h) {
                    
                    std::string manualContent = "Could not load manual.txt";
                    std::ifstream manualFile("assets/manual.txt");
                    if (manualFile.is_open()) {
                        std::stringstream buffer;
                        buffer << manualFile.rdbuf();
                        manualContent = buffer.str();
                        manualFile.close();
                    } else {
                        Logger::getInstance().log("Failed to load assets/manual.txt");
                    }
                        
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Manual", manualContent.c_str(), window);
                    Logger::getInstance().log("Manual Opened.");
                }
                
                // Check Quit
                if (mx >= quitRect.x && mx <= quitRect.x + quitRect.w &&
                    my >= quitRect.y && my <= quitRect.y + quitRect.h) {
                    isRunning = false;
                }
            } else if (gameState == PLAYING && level) {
                 // Pass click to Level
                 if (level) level->handleMouseClick((int)mx, (int)my);
            }
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
        SDL_RenderTexture(renderer, btnManual, nullptr, &manualRect);
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
    SDL_DestroyTexture(btnManual);
    SDL_DestroyTexture(btnQuit);
    
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    window = nullptr;
    renderer = nullptr;
    isRunning = false;
    SDL_Quit();
    Logger::getInstance().log("Game cleaned");

}

std::ostream& operator<<(std::ostream& os, const Game& game)
{
    os << "Game [Running: " << game.isRunning << ", State: " << game.gameState << "]";
    return os;
}
