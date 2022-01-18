//
//  CollisionSystem.hpp
//  Pong
//
//  Created by Joseph Gu on 1/12/22.
//

#ifndef CollisionSystem_hpp
#define CollisionSystem_hpp

#include <memory>
#include "CollisionComponent.hpp"
#include <stdio.h>
#include "Observer.hpp"

class CollisionSystem : public Observer, public std::enable_shared_from_this<CollisionSystem> {
private:
    std::vector<std::weak_ptr<CollisionComponent>> collidables;
    void handleCollision(CollisionComponent* ccone, CollisionComponent* cctwo);
    inline bool handleCollisionOneAxis(const AxisBounds& abone, const AxisBounds& abtwo) {
        if (abone.max > abtwo.max) {
            return (abone.min < abtwo.max);
        } 
        if (abtwo.max > abone.max) {
            return (abtwo.min < abone.max);
        }
        return false; 
    }
public:
    void insertCollidable(std::weak_ptr<CollisionComponent>& cc);
    void notify(const Subject &subject, GameEvent ge) override;
};

#endif /* CollisionSystem_hpp */
 
