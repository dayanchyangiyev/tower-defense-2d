#ifndef Effect_h
#define Effect_h

#include <string>
#include <memory>

// Forward declaration
class Enemy;

/**
 * @brief Base class for Status Effects.
 */
class Effect {
protected:
    int durationFrames;
    bool finished;

public:
    Effect(int frames) : durationFrames(frames), finished(false) {}
    virtual ~Effect() = default;

    virtual void apply(Enemy* enemy) = 0;
    virtual void update(Enemy* enemy) {
        (void)enemy; // Unused in base
        durationFrames--;
        if (durationFrames <= 0) finished = true;
    }
    
    bool isFinished() const { return finished; }
    
    // RTTI Helper
    virtual std::string getName() const = 0;
    
    // Clone for Deep Copy
    virtual std::unique_ptr<Effect> clone() const = 0;
    
    virtual void remove(Enemy* enemy) { (void)enemy; /* Default does nothing */ }
};

/**
 * @brief Slows down the enemy.
 */
class SlowEffect : public Effect {
    float originalSpeed;
    float slowFactor;
    bool applied;
public:
    SlowEffect(int frames, float factor) 
        : Effect(frames), originalSpeed(0), slowFactor(factor), applied(false) {}
        
    void apply(Enemy* enemy) override;
    void update(Enemy* enemy) override;
    std::string getName() const override { return "Slow"; }
    
    std::unique_ptr<Effect> clone() const override {
        return std::make_unique<SlowEffect>(durationFrames, slowFactor);
    }
    
    // Reset speed when effect ends
    // Reset speed when effect ends
    void remove(Enemy* enemy) override;
};

/**
 * @brief Burns the enemy (DoT).
 */
class BurnEffect : public Effect {
    int damagePerTick;
public:
    BurnEffect(int frames, int dmg) : Effect(frames), damagePerTick(dmg) {}
    
    void apply(Enemy* enemy) override; // Initial burst?
    void update(Enemy* enemy) override;
    std::string getName() const override { return "Burn"; }
    
    std::unique_ptr<Effect> clone() const override {
        return std::make_unique<BurnEffect>(durationFrames, damagePerTick);
    }
};

#endif /* Effect_h */
