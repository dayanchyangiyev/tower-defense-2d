#ifndef TowerFactory_h
#define TowerFactory_h

#include "Towers.h"
#include <memory> 

enum class TowerType {
    Basic,
    Ice,
    Fire
};

class TowerFactory {
public:
    static std::unique_ptr<Tower> createTower(TowerType type, Point2D pos, SDL_Renderer* ren) {
        switch(type) {
            case TowerType::Ice:
                return std::make_unique<IceTower>(pos, ren);
            case TowerType::Fire:
                return std::make_unique<FireTower>(pos, ren);
            case TowerType::Basic:
            default:
                return std::make_unique<Tower>(pos, 15, 150.0f, ren);
        }
    }
};

#endif /* TowerFactory_h */
