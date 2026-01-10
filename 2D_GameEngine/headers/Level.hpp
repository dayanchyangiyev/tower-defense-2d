#ifndef Level_hpp
#define Level_hpp

#include <vector>
#include <memory>
#include <SDL3/SDL.h>
#include "GameObject.h"
#include "Map.hpp"
#include "TowerFactory.h"

/**
 * @brief Manages a single game level, including map and objects.
 */
class Level {
public:
    Level(SDL_Renderer* ren, int wave);
    ~Level(); // Manage map
    
    // Disable copy/move due to managed resources
    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;
    
    // User interaction
    void placeTower(int x, int y);
    void handleInput(SDL_Keycode key);
    void handleMouseClick(int x, int y);
    void loadMap(int arr[20][25]);
    
    void update();
    void render();

private:
    void renderCursor();
    
    // Polymorphic container (Smart Pointers)
    std::vector<std::unique_ptr<GameObject>> objects;
    
    // Helpers
    std::vector<Enemy*> getEnemies();
    std::vector<Tower*> getTowers();

    int cursorX, cursorY; 
    int towersPlaced;
    // MAX_TOWERS moved to cpp implementation to force update
    
    int gameTimerFrames; 
    bool gameOver;
    bool gameWon;
    
    // Level specific
    SDL_Renderer* renderer;
    Map* map;
    int currentWave;
    int frameCount;
    int spawnTimer;
    
    // UI Logic
    TowerType selectedTowerType = TowerType::Basic;
    
    friend std::ostream& operator<<(std::ostream& os, const Level& level);
};

#endif /* Level_hpp */
