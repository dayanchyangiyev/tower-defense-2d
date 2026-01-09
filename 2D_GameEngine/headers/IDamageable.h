#ifndef IDamageable_h
#define IDamageable_h

/**
 * @brief Interface for objects that can take damage.
 * 
 * Provides a contract for taking damage, checking health status,
 * and retrieving current health. Used via dynamic_cast to interact
 * with unknown GameObjects.
 */
class IDamageable {
public:
    virtual ~IDamageable() = default;

    /**
     * @brief Apply damage to the object.
     * @param amount The amount of damage to inflict.
     */
    virtual void takeDamage(int amount) = 0;

    /**
     * @brief Check if the object is still alive.
     * @return true if health > 0, false otherwise.
     */
    virtual bool isAlive() const = 0;

    /**
     * @brief Get current health value.
     * @return Current health integer.
     */
    virtual int getHealth() const = 0;
};

#endif /* IDamageable_h */
