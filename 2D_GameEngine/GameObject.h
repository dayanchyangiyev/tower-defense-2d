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


class GameException : public std::exception {
protected:
    std::string message;
public:
    explicit GameException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class ResourceError : public GameException {
public:
    explicit ResourceError(const std::string& msg) : GameException("Resource Error: " + msg) {}
};

class InitializationError : public GameException {
public:
    explicit InitializationError(const std::string& msg) : GameException("Init Error: " + msg) {}
};

class LogicError : public GameException {
public:
    explicit LogicError(const std::string& msg) : GameException("Logic Error: " + msg) {}
};

// --- Point2D Helper ---
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



// --- GameObject Base Class ---
class GameObject {
public:
    GameObject(const char* texturesheet, SDL_Renderer* ren, float x, float y);
    virtual ~GameObject();

    // Copy-Swap Idiom - changed to standard assignment for abstract base
    GameObject(const GameObject& other);
    GameObject& operator=(const GameObject& other); 
    // friend void swap(GameObject& first, GameObject& second) noexcept; // removed swap friend for now or strictly used internally

    // Virtual Constructor
    virtual std::unique_ptr<GameObject> clone() const = 0;

    // TEMA 2: Virtual function specific to theme
    virtual void onClick() { /* Default: do nothing */ }

    // Pure Virtuals
    virtual void update() = 0;
    virtual void render() = 0;

    // NVI for printing
    friend std::ostream& operator<<(std::ostream& os, const GameObject& obj);

    // Getters/Setters
    float getX() const { return xpos; }
    float getY() const { return ypos; }
    Point2D getPos() const { return Point2D(xpos, ypos); }
    void setPos(float x, float y) { xpos = x; ypos = y; }
    bool isActive() const { return active; }
    void setActive(bool a) { active = a; }

protected:
    virtual void print(std::ostream& os) const; // For NVI

    float xpos;
    float ypos;
    int width;
    int height;
    bool active;
    
    SDL_Renderer* renderer; 
    std::string texturePath;
    SDL_Texture* objTexture;
    SDL_FRect srcRect{}, destRect{};
};

// --- Derived Class 1: Enemy ---
class Enemy : public GameObject {
private:
    std::string name; 
    int health;
    int maxHealth;
    float speed;
    Point2D targetPos;

public:
    Enemy(const char* name, Point2D startPos, int health, float speed, SDL_Renderer* ren);
    
    // Clone
    std::unique_ptr<GameObject> clone() const override;

    void update() override;
    void render() override;
    void onClick() override; // TEMA 2 Specific

    void setTarget(float x, float y);
    void setSpeed(float s) { speed = s; }
    void takeDamage(int amount);
    
    // Enforcing strict setters? 
    // TEMA 2 says "high level functions, eliminate getters/setters".
    // Logic is mostly inside update/takeDamage.
    
    // Helpers
    std::string getName() const { return name; }
    bool isAlive() const { return health > 0; }

protected:
    void print(std::ostream& os) const override;
};

// --- Derived Class 2: Tower ---
class Tower : public GameObject {
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
    
    void attack(Enemy& enemy);
    void upgrade();
    void takeDamage(int amount);
    bool canAttack(const Enemy& enemy) const;
    bool isAlive() const { return health > 0; }
    int getDamage() const { return damage; }

protected:
    void print(std::ostream& os) const override;
};

// --- Derived Class 3: Projectile (New) ---
class Projectile : public GameObject {
private:
    float speed;
    Point2D target;
    // We might need a target pointer or just position.
    // Simple logic: moves to target, deactivates.

public:
    Projectile(Point2D start, Point2D target, float speed, SDL_Renderer* ren);
    
    std::unique_ptr<GameObject> clone() const override;

    void update() override;
    void render() override;

protected:
    void print(std::ostream& os) const override;
};

// --- Derived Class 4: Explosion (New) ---
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