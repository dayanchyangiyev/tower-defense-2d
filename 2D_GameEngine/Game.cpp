#include "Game.hpp"

SDL_Texture* playerTex; 

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if(fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if(SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "Subsystem Initialised!..." << std::endl;

        window = SDL_CreateWindow(title, width, height, static_cast<SDL_WindowFlags>(flags)); //window initialization
        if(window)
        {
            std::cout << "window created!" << std::endl;
            SDL_SetWindowPosition(window, xpos, ypos);
        }

        renderer = SDL_CreateRenderer(window, NULL);
        if(renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created!" << std::endl;
        }
        isRunning = true;

    } else {
        isRunning = false;
    }


    SDL_Surface* tmpSurface = IMG_Load("assets/tower.png");
    playerTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_DestroySurface(tmpSurface);
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event); //to record the event
    switch (event.type) {
        case SDL_EVENT_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }

}
void Game::update()
{}

void Game::render()
{
    SDL_RenderClear(renderer);
    //this is where we would add stuff to render
    SDL_RenderTexture(renderer, playerTex, NULL, NULL);
    SDL_RenderPresent((renderer));
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;

}
