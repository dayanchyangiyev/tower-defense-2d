#include "GameObject.h"
#include "TextureManager.h"
#include "Logger.hpp"
#include <cstring>
#include <sstream>

int GameObject::objectCount = 0;

int GameObject::getCount() {
    return objectCount;
}

//  Point2D
std::ostream& operator<<(std::ostream& os, const Point2D& point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

// Static Helpers implementation
float GameObject::distance(const GameObject& a, const GameObject& b) {
    return std::sqrt(std::pow(a.xPos - b.xPos, 2) + std::pow(a.yPos - b.yPos, 2));
}

bool GameObject::checkCollision(const GameObject& a, const GameObject& b) {
    return (a.xPos < b.xPos + b.width &&
            a.xPos + a.width > b.xPos &&
            a.yPos < b.yPos + b.height &&
            a.yPos + a.height > b.yPos);
}

// GameObject
GameObject::GameObject(const char* textureSheet, SDL_Renderer* ren, float x, float y)
    : xPos(x), yPos(y), width(32), height(32), active(true), renderer(ren), texturePath(textureSheet ? textureSheet : "")
{
    objectCount++;
    if (renderer && !texturePath.empty()) {
        try {
            objTexture = TextureManager::LoadTexture(textureSheet, ren);
        } catch (const ResourceError& e) {
            Logger::log(e.what());
            // Fallback or rethrow? 
            // We can continue with null texture (invisible object) but log it.
            objTexture = nullptr; 
        }
    } else {
        objTexture = nullptr;
    }
}

GameObject::GameObject(const GameObject& other)
    : xPos(other.xPos), yPos(other.yPos), width(other.width), height(other.height), 
      active(other.active), renderer(other.renderer), texturePath(other.texturePath)
{
    objectCount++;
    if (renderer && !texturePath.empty()) {
        try {
            objTexture = TextureManager::LoadTexture(texturePath.c_str(), renderer);
        } catch (...) {
            objTexture = nullptr;
        }
    } else {
        objTexture = nullptr;
    }
}

GameObject& GameObject::operator=(const GameObject& other) {
    if (this == &other) return *this;
    
    xPos = other.xPos;
    yPos = other.yPos;
    width = other.width;
    height = other.height;
    active = other.active;
    renderer = other.renderer;
    texturePath = other.texturePath;
    
    if (objTexture) {
        SDL_DestroyTexture(objTexture);
        objTexture = nullptr;
    }

    if (renderer && !texturePath.empty()) {
        try {
            objTexture = TextureManager::LoadTexture(texturePath.c_str(), renderer);
        } catch (...) {
            objTexture = nullptr;
        }
    }
    
    return *this;
}



GameObject::~GameObject() {
    objectCount--;
    if (objTexture) {
        SDL_DestroyTexture(objTexture);
    }
}

void GameObject::print(std::ostream& os) const {
    os << "GameObject @" << getPos();
}

std::ostream& operator<<(std::ostream& os, const GameObject& obj) {
    obj.print(os);
    return os;
}

void GameObject::onCollision(GameObject& /*other*/) {
    // Default implementation: log collision
    // Logger::log("Collision detected!");
}

// Enemy
Enemy::Enemy(const char* name, Point2D startPos, int health, float speed, SDL_Renderer* ren)
    : GameObject("assets/enemy.bmp", ren, startPos.getX(), startPos.getY()),
      name(name ? name : "Unknown"), health(health), maxHealth(health), speed(speed)
{}

std::unique_ptr<Enemy> Enemy::createGoblin(SDL_Renderer* ren, int x, int y) {
    return std::make_unique<Enemy>("Goblin", Point2D(x, y), 80, 1.5f, ren);
}

std::unique_ptr<Enemy> Enemy::createOrc(SDL_Renderer* ren, int x, int y) {
    return std::make_unique<Enemy>("Orc", Point2D(x, y), 150, 0.8f, ren);
}

std::unique_ptr<GameObject> Enemy::clone() const {
    return std::make_unique<Enemy>(*this);
}

void Enemy::update() {
    if (!active) return;
    
    // Move logic
    float dx = targetPos.getX() - xPos;
    float dy = targetPos.getY() - yPos;
    float dist = std::sqrt(dx*dx + dy*dy);
    
    if (dist > speed) {
        xPos += (dx/dist) * speed;
        yPos += (dy/dist) * speed;
    }
    
    // Update rects
    srcRect = {0, 0, 32, 32};
    destRect = {xPos, yPos, (float)width, (float)height};
}

void renderHealthBar(SDL_Renderer* ren, float x, float y, int hp, int maxHp) {
    if (!ren) return;
    SDL_FRect bgRect = {x, y - 10, 32.0f, 5.0f};
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderFillRect(ren, &bgRect);
    
    float percentage = (float)hp / (float)maxHp;
    if (percentage < 0) percentage = 0;
    SDL_FRect fgRect = {x, y - 10, 32.0f * percentage, 5.0f};
    SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
    SDL_RenderFillRect(ren, &fgRect);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
}

void Enemy::render() {
    if (active) {
        if (objTexture) SDL_RenderTexture(renderer, objTexture, &srcRect, &destRect);
        renderHealthBar(renderer, xPos, yPos, health, maxHealth);
    }
}

void Enemy::onClick() {
    Logger::log("Clicked on Enemy: " + name);
}

void Enemy::setTarget(float x, float y) {
    targetPos.setX(x); targetPos.setY(y);
}

void Enemy::takeDamage(int amount) {
    if (!active) return;
    health -= amount;
    if (health <= 0) {
        health = 0; active = false;
        Logger::log("Enemy " + name + " defeated!");
    }
}

void Enemy::print(std::ostream& os) const {
    os << "Enemy [" << name << "] @" << getPos();
}


//  Tower
Tower::Tower(Point2D pos, int damage, float range, SDL_Renderer* ren)
    : GameObject("assets/tower.bmp", ren, pos.getX(), pos.getY()),
      damage(damage), range(range), level(1), health(100)
{}

std::unique_ptr<GameObject> Tower::clone() const {
    return std::make_unique<Tower>(*this);
}

void Tower::update() {
    // Logic can trigger here if needed
    srcRect = {0, 0, 32, 32};
    destRect = {xPos, yPos, (float)width, (float)height};
}

void Tower::render() {
    if (objTexture) SDL_RenderTexture(renderer, objTexture, &srcRect, &destRect);
}

void Tower::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void Tower::upgrade() {
    level++;
    damage += 2; range += 10.0f;
    Logger::log("Tower upgraded!");
}

bool Tower::canAttack(const Enemy& enemy) const {
    if (!enemy.isAlive()) return false;
    return getPos().distanceTo(enemy.getPos()) <= range;
}

void Tower::attack(Enemy& enemy) {
    if (canAttack(enemy)) {
        enemy.takeDamage(damage);
        // Visual effect can be spawning a projectile here! implemented in Level.
    }
}

void Tower::print(std::ostream& os) const {
    os << "Tower [Lvl:" << level << "] @" << getPos();
}

// Projectile
Projectile::Projectile(Point2D start, Point2D target, float speed, SDL_Renderer* ren)
    : GameObject("assets/projectile.png", ren, start.getX(), start.getY()), // Assuming asset exists or will fail
      speed(speed), target(target)
{
    // Try to load a simple shape if texture fails?
    // We already handle exception in base.
}

std::unique_ptr<GameObject> Projectile::clone() const {
    return std::make_unique<Projectile>(*this);
}

void Projectile::update() {
    if (!active) return;
    
    // Check collisions in Level loop generally, or here if we have access to scene?
    // Projectile logic is usually handled by Level loop detecting collisions.
    
    float dx = target.getX() - xPos;
    float dy = target.getY() - yPos;
    float dist = std::sqrt(dx*dx + dy*dy);
    
    if (dist < speed) {
        xPos = target.getX();
        yPos = target.getY();
        active = false; // Reached target
    } else {
        xPos += (dx/dist) * speed;
        yPos += (dy/dist) * speed;
    }
    
    srcRect = {0, 0, 32, 32}; // Assume small sprite or reuse
    destRect = {xPos, yPos, 16.0f, 16.0f}; // Smaller
}

void Projectile::render() {
    if (active && objTexture) {
        SDL_RenderTexture(renderer, objTexture, &srcRect, &destRect);
    } else if (active) {
        // Fallback render (yellow dot)
        SDL_FRect r = {xPos, yPos, 8, 8};
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &r);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
}

void Projectile::print(std::ostream& os) const {
    os << "Projectile @" << getPos();
}

// Explosion
Explosion::Explosion(Point2D pos, SDL_Renderer* ren) 
    : GameObject("assets/explosion.bmp", ren, pos.getX(), pos.getY()), life(10)
{}

std::unique_ptr<GameObject> Explosion::clone() const {
    return std::make_unique<Explosion>(*this);
}

void Explosion::update() {
    life--;
    if (life <= 0) active = false;
    srcRect = {0, 0, 32, 32};
    destRect = {xPos, yPos, 32.0f, 32.0f};
}

void Explosion::render() {
    if (active) {
        if (objTexture) {
             SDL_RenderTexture(renderer, objTexture, &srcRect, &destRect);
        } else {
             // Fallback: Red square
             SDL_FRect r = {xPos, yPos, 32.0f, 32.0f};
             SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255); // Orange
             SDL_RenderFillRect(renderer, &r);
             SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
    }
}

void Explosion::print(std::ostream& os) const {
    os << "Explosion @" << getPos();
}