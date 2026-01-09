#include "Level.hpp"
#include "Logger.hpp"
#include "Utils.hpp"
#include "EnemyFactory.h"
#include "Matrix2D.hpp"
#include <sstream>

// Second instantiation of template class as requested (dummy usage)
Matrix2D<float, 20, 25> dangerMap;

Level::Level(SDL_Renderer* ren, int wave) 
    : cursorX(12), cursorY(10), towersPlaced(0), gameTimerFrames(0), gameOver(false), gameWon(false), 
      renderer(ren), map(nullptr), currentWave(wave), frameCount(0), spawnTimer(0) 
{
    map = new Map(ren);
    // Polymorphic load could go here
}

Level::~Level() {
    delete map;
    // objects cleared automatically by unique_ptr
}

void Level::loadMap(int arr[20][25]) {
    map->LoadMap(arr);
}

// Helper to filter objects by type
// Using IDamageable interface check where appropriate would be better design, but for specific list access:
std::vector<Enemy*> Level::getEnemies() {
    std::vector<Enemy*> enemies;
    for(auto& obj : objects) {
        // Safe check using dynamic_cast as requested for meaningful RTTI
        if(auto e = dynamic_cast<Enemy*>(obj.get())) {
            if(e->isActive()) enemies.push_back(e);
        }
    }
    return enemies;
}

std::vector<Tower*> Level::getTowers() {
    std::vector<Tower*> towers;
    for(auto& obj : objects) {
        if(auto t = dynamic_cast<Tower*>(obj.get())) {
            if(t->isActive()) towers.push_back(t);
        }
    }
    return towers;
}

void Level::placeTower(int x, int y) {
    // Grid coords are internal logic
    int col = x; 
    int row = y;
    
    // Pixel/Grid conversion
    if (x > 25 && y > 25) { 
        col = x / 32;
        row = y / 32;
    }

    float tx = col * 32.0f;
    float ty = row * 32.0f;
    
    if (towersPlaced < MAX_TOWERS) {
        towersPlaced++;
        // Polymorphic Add: Tower
        objects.push_back(std::make_unique<Tower>(Point2D(tx, ty), 50, 150.0f, renderer));
        
        std::stringstream ss;
        ss << "Placed tower at grid (" << col << ", " << row << ")";
        Logger::getInstance().log(ss.str());
    } else {
        Logger::getInstance().log("Max towers reached!");
    }
}

void Level::handleInput(SDL_Keycode key) {
    if (gameOver) return;
    
    switch (key) {
        case SDLK_UP:
            if (cursorY > 0) cursorY--;
            break;
        case SDLK_DOWN:
            if (cursorY < 19) cursorY++;
            break;
        case SDLK_LEFT:
            if (cursorX > 0) cursorX--;
            break;
        case SDLK_RIGHT:
            if (cursorX < 24) cursorX++;
            break;
        case SDLK_RETURN:
            placeTower(cursorX, cursorY);
            break;
    }
}

void Level::update() {
    if (gameOver) return;

    // Timer
    gameTimerFrames++;
    if (gameTimerFrames >= 30 * 60) { 
        gameOver = true;
        gameWon = true; 
        Logger::getInstance().log("Time is up! You survived!");
    }

    //  Prep Phase
    if (gameTimerFrames < 10 * 30) {
        if (gameTimerFrames % 30 == 0) {
            std::stringstream ss;
            ss << "Prep Phase: " << (10 - gameTimerFrames/30) << "s remaining. Place towers!";
            Logger::getInstance().log(ss.str());
        }
    } else {
        // Spawn Logic
        spawnTimer++;
        if (spawnTimer >= 150) {
            int side = rand() % 4;
            int sx, sy;
            switch(side) {
                case 0: sx = rand() % 800; sy = 0; break; 
                case 1: sx = rand() % 800; sy = 600; break; 
                case 2: sx = 0; sy = rand() % 600; break; 
                case 3: sx = 800; sy = rand() % 600; break; 
            }
            
            // Polymorphic Add: Enemy using Factory Pattern
            std::unique_ptr<Enemy> e;
            if (rand() % 2 == 0) {
                 e = EnemyFactory::createGoblin(renderer, sx, sy);
            } else {
                 e = EnemyFactory::createOrc(renderer, sx, sy);
            }

            e->setTarget(400, 300); // Default Center
            objects.push_back(std::move(e));
            
            spawnTimer = 0;
        }
    }
    
    // Polymorphic Update
    // Iterate and update all objects (Towers, Enemies, Projectiles)
    for(auto& obj : objects) {
        obj->update();
    }
    
    // Logic / AI Update
    // We need to fetch filtered lists to interact
    auto enemies = getEnemies();
    auto towers = getTowers();
    
    // 1. Enemy AI: Target Towers (Using template function findNearest)
    for(auto* enemy : enemies) {
        // Find nearest Tower to attack
        // Instantiation 1 of template function
        Tower* targetTower = Utils::findNearest<Tower>(*enemy, objects, 99999.0f);
        
        if (targetTower) {
            enemy->setTarget(targetTower->getX(), targetTower->getY());
        } else {
            enemy->setTarget(400, 300); // Default to center if no towers
        }
        
        // Terrain Speed
        int r = (int)enemy->getY() / 32;
        if (r == 10) enemy->setSpeed(1.0f);
        else enemy->setSpeed(2.5f);
        
        // Attack Tower if close
        if (targetTower) {
             float dist = GameObject::distance(*enemy, *targetTower);
             if (dist < 32.0f) {
                 // Meaningful cast for logic
                 IDamageable* dmgObj = dynamic_cast<IDamageable*>(targetTower);
                 if (dmgObj && frameCount == 0) {
                     dmgObj->takeDamage(1); 
                 }
             }
        }
    }
    
    // 2. Tower AI: Attack Enemies
    frameCount++;
    if (frameCount >= 30) { 
        for(auto* tower : towers) {
            // Instantiation 2 of template function
            Enemy* nearestEnemy = Utils::findNearest<Enemy>(*tower, objects, tower->getHealth() > 50 ? 200.0f : 150.0f); // Range depends on health just for fun
            
            if (nearestEnemy && tower->canAttack(*nearestEnemy)) {
                tower->attack(*nearestEnemy);
                // Spawn Projectile (Visual)
                Point2D startP = tower->getPos();
                Point2D endP = nearestEnemy->getPos();
                    
                    // Use static distance helper
                    // float d = GameObject::distance(*tower, *enemy);
                    
                objects.push_back(std::unique_ptr<GameObject>(
                    new Projectile(startP, endP, 10.0f, renderer)
                ));
                // Add Explosion (Muzzle Flash)
                objects.push_back(std::unique_ptr<GameObject>(
                    new Explosion(startP, renderer)
                ));
            }
        }
        frameCount = 0;
    }

    // Cleanup Dead Object
    // Custom predicate dealing with unique_ptr
    std::erase_if(objects, [](const auto& obj){ 
        return !obj->isActive(); // Assuming base GameObject has isActive
        
    });
    
    // Update Title
    SDL_Window* win = SDL_GetRenderWindow(renderer);
    if (win) {
        std::stringstream titleSS;
        int seconds = gameTimerFrames / 30;
        int prepLeft = 10 - seconds;
        if (prepLeft > 0) titleSS << "Tower Defense - PREP: " << prepLeft << "s";
        else titleSS << "Tower Defense - SURVIVE: " << (seconds - 10) << "s";
        SDL_SetWindowTitle(win, titleSS.str().c_str());
    }
}

void Level::render() {
    map->DrawMap();
    
    // Polymorphic Render
    // Sort by Y for pseudodepth? Optional.
    for(auto& obj : objects) {
        obj->render();
    }
    
    renderCursor();
    
    if (gameOver) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_FRect overlay = {0, 0, 800, 600};
        SDL_RenderFillRect(renderer, &overlay);
    }
}

void Level::renderCursor() {
    SDL_FRect r = {cursorX * 32.0f, cursorY * 32.0f, 32.0f, 32.0f};
    SDL_SetRenderDrawColor(renderer, 200, 200, 255, 150); 
    SDL_RenderFillRect(renderer, &r);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}


