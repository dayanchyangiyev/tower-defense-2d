#ifndef Towers_h
#define Towers_h

#include "GameObject.h"
#include "Effect.h"
#include "Logger.hpp"

/**
 * @brief Tower that applies SlowEffect.
 */
class IceTower : public Tower {
public:
    IceTower(Point2D pos, SDL_Renderer* ren) 
        : Tower(pos, 5, 800.0f, ren) // Huge range (80% of map)
    {
        // Visual distinction: BLUE (0, 0, 255)
        if (objTexture) {
            SDL_SetTextureColorMod(objTexture, 0, 0, 255);
        }
    }
    
    // Override Attack to apply effect
    void attack(Enemy& enemy) override {
        // Call base damage logic first (optional, or custom)
        // Tower::attack(enemy) deals damage.
        // But we want to also apply effect.
        if (canAttack(enemy)) {
             // Base damage
             Tower::attack(enemy);
             
             // Apply Slow
             // 60 frames = 2 seconds, 0.5 factor
             enemy.addEffect(std::make_unique<SlowEffect>(60, 0.5f));
             // Logger::getInstance().log("IceTower hit!");
        }
    }
    
    SDL_Color getProjectileColor() const override { return {0, 255, 255, 255}; } // Cyan
};

/**
 * @brief Tower that applies BurnEffect.
 */
class FireTower : public Tower {
public:
    FireTower(Point2D pos, SDL_Renderer* ren) 
        : Tower(pos, 10, 150.0f, ren) // High damage
    {
        // Visual distinction: ORANGE (255, 165, 0)
        if (objTexture) {
             SDL_SetTextureColorMod(objTexture, 255, 165, 0);
        }
    }
    
    std::unique_ptr<GameObject> clone() const override {
         return std::make_unique<FireTower>(*this);
    }
    
    void attack(Enemy& enemy) override {
        if (canAttack(enemy)) {
             Tower::attack(enemy);
             // Apply Burn
             // 90 frames = 3 seconds, 2 damage per tick
             enemy.addEffect(std::make_unique<BurnEffect>(90, 2));
             // Logger::getInstance().log("FireTower hit!");
        }
    }
    
    SDL_Color getProjectileColor() const override { return {255, 165, 0, 255}; } // Orange
};

#endif /* Towers_h */
