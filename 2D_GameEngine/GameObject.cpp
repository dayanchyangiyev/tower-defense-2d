#include "GameObject.h"
#include "Logger.hpp"
#include <cstring>
#include <sstream>

// --- Point2D ---
std::ostream& operator<<(std::ostream& os, const Point2D& point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

// --- GameObject ---
GameObject::GameObject(const char* texturesheet, SDL_Renderer* ren, float x, float y)
    : xpos(x), ypos(y), width(32), height(32), active(true), renderer(ren), texturePath(texturesheet ? texturesheet : "")
{
    if (renderer && !texturePath.empty()) {
        try {
            objTexture = TextureManager::LoadTexture(texturesheet, ren);
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
    : xpos(other.xpos), ypos(other.ypos), width(other.width), height(other.height), 
      active(other.active), renderer(other.renderer), texturePath(other.texturePath)
{
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
    
    xpos = other.xpos;
    ypos = other.ypos;
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

// void swap(GameObject& first, GameObject& second) noexcept { ... } // Removed

GameObject::~GameObject() {
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

// --- Enemy ---
Enemy::Enemy(const char* name, Point2D startPos, int health, float speed, SDL_Renderer* ren)
    : GameObject("assets/enemy.bmp", ren, startPos.getX(), startPos.getY()),
      name(name ? name : "Unknown"), health(health), maxHealth(health), speed(speed)
{}

std::unique_ptr<GameObject> Enemy::clone() const {
    return std::make_unique<Enemy>(*this);
}

void Enemy::update() {
    if (!active) return;
    
    // Move logic
    float dx = targetPos.getX() - xpos;
    float dy = targetPos.getY() - ypos;
    float dist = std::sqrt(dx*dx + dy*dy);
    
    if (dist > speed) {
        xpos += (dx/dist) * speed;
        ypos += (dy/dist) * speed;
    }
    
    // Update rects
    srcRect = {0, 0, 32, 32};
    destRect = {xpos, ypos, (float)width, (float)height};
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
        renderHealthBar(renderer, xpos, ypos, health, maxHealth);
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


// --- Tower ---
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
    destRect = {xpos, ypos, (float)width, (float)height};
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

// --- Projectile ---
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
    
    float dx = target.getX() - xpos;
    float dy = target.getY() - ypos;
    float dist = std::sqrt(dx*dx + dy*dy);
    
    if (dist < speed) {
        xpos = target.getX();
        ypos = target.getY();
        active = false; // Reached target
    } else {
        xpos += (dx/dist) * speed;
        ypos += (dy/dist) * speed;
    }
    
    srcRect = {0, 0, 32, 32}; // Assume small sprite or reuse
    destRect = {xpos, ypos, 16.0f, 16.0f}; // Smaller
}

void Projectile::render() {
    if (active && objTexture) {
        SDL_RenderTexture(renderer, objTexture, &srcRect, &destRect);
    } else if (active) {
        // Fallback render (yellow dot)
        SDL_FRect r = {xpos, ypos, 8, 8};
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &r);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
}

void Projectile::print(std::ostream& os) const {
    os << "Projectile @" << getPos();
}

// --- Explosion ---
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
    destRect = {xpos, ypos, 32.0f, 32.0f};
}

void Explosion::render() {
    if (active) {
        if (objTexture) {
             SDL_RenderTexture(renderer, objTexture, &srcRect, &destRect);
        } else {
             // Fallback: Red square
             SDL_FRect r = {xpos, ypos, 32.0f, 32.0f};
             SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255); // Orange
             SDL_RenderFillRect(renderer, &r);
             SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
    }
}

void Explosion::print(std::ostream& os) const {
    os << "Explosion @" << getPos();
}