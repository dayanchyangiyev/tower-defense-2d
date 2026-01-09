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

}

#endif /* Utils_hpp */
