#ifndef Utils_hpp
#define Utils_hpp

#include <vector>
#include <memory>
#include <cmath>
#include "GameObject.h"

namespace Utils {

/**
 * @brief Template function to find the nearest object of type T from a collection.
 * 
 * @tparam T The specific type of GameObject to find (Enemy, Tower).
 * @param source The source object to measure distance from.
 * @param objects The collection of objects (pointers).
 * @param range Max range to search.
 * @return T* Pointer to the nearest object or nullptr.
 */
template <typename T>
T* findNearest(const GameObject& source, const std::vector<std::unique_ptr<GameObject>>& objects, float range) {
    T* nearest = nullptr;
    float minD = range;
    
    for(const auto& obj : objects) {
        if(!obj->isActive()) continue;
        
        // Check exact type match via dynamic_cast (RTTI) as requested
        T* typedObj = dynamic_cast<T*>(obj.get());
        if(typedObj) {
            float d = GameObject::distance(source, *typedObj);
            if(d <= minD) {
                minD = d;
                nearest = typedObj;
            }
        }
    }
    return nearest;
}

struct MathUtils {
    static float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }
    
    static float clamp(float val, float min, float max) {
        if(val < min) return min;
        if(val > max) return max;
        return val;
    }
    
    static double angleBetween(float x1, float y1, float x2, float y2) {
        return std::atan2(y2 - y1, x2 - x1) * 180.0 / M_PI;
    }
};

}

#endif /* Utils_hpp */
