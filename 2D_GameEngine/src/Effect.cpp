#include "Effect.h"
#include "GameObject.h"
#include "Logger.hpp"

// SlowEffect
void SlowEffect::apply(Enemy* enemy) {
    if (!applied) {
        // We need to access speed, assuming getSpeed/setSpeed exists or friend
        // For now, let's assume we can modify speed via setSpeed
        originalSpeed = enemy->getSpeed();
        enemy->setSpeed(originalSpeed * slowFactor); 
        applied = true;
        // Logger::getInstance().log("Slow applied!");
    }
}

void SlowEffect::update(Enemy* enemy) {
    Effect::update(enemy);
    // Continuously enforce or just wait for end?
    if (finished) remove(enemy);
}

void SlowEffect::remove(Enemy* enemy) {
    if (applied) {
        enemy->setSpeed(originalSpeed); // Restore original
        applied = false;
        // Logger::getInstance().log("Slow removed!");
    }
}

// BurnEffect
void BurnEffect::apply(Enemy* enemy) {
    (void)enemy;
    // maybe initial hit
}

void BurnEffect::update(Enemy* enemy) {
    Effect::update(enemy);
    if (durationFrames % 30 == 0) { // Every second
         enemy->takeDamage(damagePerTick);
         // Logger::getInstance().log("Burn tick!");
    }
}
