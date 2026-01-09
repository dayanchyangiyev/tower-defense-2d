#ifndef EnemyFactory_h
#define EnemyFactory_h

#include "GameObject.h"
#include <memory>

/**
 * @brief Factory class for creating enemies.
 * Implements the Factory Method design pattern.
 */
class EnemyFactory {
public:
    static std::unique_ptr<Enemy> createGoblin(SDL_Renderer* ren, int x, int y) {
        // Delegate to Enemy static creation helper for now
        // Or implement logic here to purify the pattern
        return Enemy::createGoblin(ren, x, y);
    }
    
    static std::unique_ptr<Enemy> createOrc(SDL_Renderer* ren, int x, int y) {
        return Enemy::createOrc(ren, x, y);
    }
};

#endif /* EnemyFactory_h */
