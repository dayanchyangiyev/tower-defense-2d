#ifndef GameObject_h
#define GameObject_h

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>
#include <exception>
#include "IDamageable.h"

// Forward check
class Effect;
class GameException : public std::exception {
protected:
    std::string message;
public:
    explicit GameException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

/**
 * @brief Exception for resource loading failures (e.g., textures).
 */
class ResourceError : public GameException {
public:
    explicit ResourceError(const std::string& msg) : GameException("Resource Error: " + msg) {}
};

/**
 * @brief Exception for initialization failures.
 */
class InitializationError : public GameException {
public:
    explicit InitializationError(const std::string& msg) : GameException("Init Error: " + msg) {}
};

/**
 * @brief Exception for logic errors (e.g., invalid states).
 */
class LogicError : public GameException {
public:
    explicit LogicError(const std::string& msg) : GameException("Logic Error: " + msg) {}
};

// Point2D Helper
class Point2D {
private:
    float x, y;
public:
    Point2D() : x(0), y(0) {}
    Point2D(float x, float y) : x(x), y(y) {}
    
    float getX() const { return x; }
    float getY() const { return y; }
    void setX(float val) { x = val; }
    void setY(float val) { y = val; }
    
    float distanceTo(const Point2D& other) const {
        return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
    }

    friend std::ostream& operator<<(std::ostream& os, const Point2D& point);
};



// GameObject Base Class 

/**
 * @brief Base class for all game entities.
 */
class GameObject {
public:
    GameObject(const char* textureSheet, SDL_Renderer* ren, float x, float y);
    virtual ~GameObject();

    GameObject(const GameObject& other);
    GameObject& operator=(const GameObject& other); 
    
    // Virtual Constructor
    virtual std::unique_ptr<GameObject> clone() const = 0;

    virtual void onClick() { /* Default: do nothing */ }

    // Pure Virtuals
    virtual void update() = 0;
    virtual void render() = 0;
    
    /**
     * @brief Handle collision with another object.
     * @param other The object collided with.
     */
    virtual void onCollision(GameObject& other);

    // NVI for printing
    friend std::ostream& operator<<(std::ostream& os, const GameObject& obj);

    // Getters/Setters
    float getX() const { return xPos; }
    float getY() const { return yPos; }
    Point2D getPos() const { return Point2D(xPos, yPos); }
    void setPos(float x, float y) { xPos = x; yPos = y; }
    bool isActive() const { return active; }
    void setActive(bool a) { active = a; }
    
    // Static member for teacher requirement
    static int getCount(); 
    
    /**
     * @brief Calculate distance between two objects.
     */
    static float distance(const GameObject& a, const GameObject& b);

    /**
     * @brief Check AABB collision between two objects.
     */
    static bool checkCollision(const GameObject& a, const GameObject& b); 

protected:
    virtual void print(std::ostream& os) const; // For NVI

    float xPos;
    float yPos;
    int width;
    int height;
    bool active;
    
    SDL_Renderer* renderer; 
    std::string texturePath;
    SDL_Texture* objTexture;
    SDL_FRect srcRect{}, destRect{};
    
    static int objectCount;
};

// Enemy class
/**
 * @brief Enemy entity that seeks a target.
 */
class Enemy : public GameObject, public IDamageable {
private:
    std::string name;  
    int health;
    int maxHealth;
    float speed;
    
    // Effects List
    std::vector<std::unique_ptr<Effect>> effects;

    Point2D targetPos;

public:
    Enemy(const char* name, Point2D startPos, int health, float speed, SDL_Renderer* ren);
    Enemy(const Enemy& other); // Deep Copy
    
    // Clone
    std::unique_ptr<GameObject> clone() const override;

    void update() override;
    void render() override;
    void onClick() override; // TEMA 2 Specific
    
    // IDamageable
    void takeDamage(int amount) override;
    bool isAlive() const override { return health > 0; }
    int getHealth() const override { return health; }

    void setTarget(float x, float y);
    void setSpeed(float s) { speed = s; }
    float getSpeed() const { return speed; }
    
    // Effect Management
    void addEffect(std::unique_ptr<Effect> effect);
    void updateEffects();
    
    // Static Factory
    static std::unique_ptr<Enemy> createGoblin(SDL_Renderer* ren, int x, int y);
    static std::unique_ptr<Enemy> createOrc(SDL_Renderer* ren, int x, int y);
    
    // Helpers
    std::string getName() const { return name; }

protected:
    void print(std::ostream& os) const override;
};

// Tower class
/**
 * @brief Tower entity that shoots projectiles.
 */
class Tower : public GameObject, public IDamageable {
private:
    int damage;
    float range;
    int level;
    int health;

public:
    Tower(Point2D pos, int damage, float range, SDL_Renderer* ren);
    
    std::unique_ptr<GameObject> clone() const override;

    void update() override;
    void render() override;
    
    // IDamageable
    void takeDamage(int amount) override;
    bool isAlive() const override { return health > 0; }
    int getHealth() const override { return health; }
    
    virtual void attack(Enemy& enemy);
    void upgrade();
    bool canAttack(const Enemy& enemy) const;
    int getDamage() const { return damage; }
    float getRange() const { return range; }
    
    virtual SDL_Color getProjectileColor() const { return {0, 0, 0, 255}; } // Default Black

protected:
    void print(std::ostream& os) const override;
};

// Projectile class
/**
 * @brief Projectile fired by towers.
 */
class Projectile : public GameObject {
private:
    float speed;
    Point2D target;
    SDL_Color color;
    

public:
    Projectile(Point2D start, Point2D target, float speed, SDL_Renderer* ren, SDL_Color col);
    
    std::unique_ptr<GameObject> clone() const override;

    void update() override;
    void render() override;
    
protected:
    void print(std::ostream& os) const override;
};

// Explosion class
/**
 * @brief Visual effects for explosions.
 */
class Explosion : public GameObject {
private:
    int life;
public:
    Explosion(Point2D pos, SDL_Renderer* ren);
    std::unique_ptr<GameObject> clone() const override;
    void update() override;
    void render() override;
    
protected:
    void print(std::ostream& os) const override;
};

#endif /* GameObject_h */