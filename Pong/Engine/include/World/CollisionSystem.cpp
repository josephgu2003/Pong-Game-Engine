//
//  CollisionSystem.cpp
//  Pong
//
//  Created by Joseph Gu on 1/12/22.
//

#include "CollisionSystem.hpp"


void CollisionSystem::insertCollidable(std::weak_ptr<CollisionComponent>& cc) {
    cc.lock()->addObserver(shared_from_this());
    collidables.emplace_back(cc);
    
}
 
void CollisionSystem::notify(const Subject &subject, GameEvent ge) {
    if (ge == SUBJ_POS_CHANGED) {
        const CollisionComponent* movedCC = &(static_cast<const CollisionComponent&>(subject));
        CollisionComponent* movedCC_ = const_cast<CollisionComponent*>(movedCC); // ???
        for (auto i = collidables.begin(); i != collidables.end(); i++) {
            if (auto otherCC = (*i).lock()) {
                if (movedCC_ != otherCC.get()) {
                    handleCollision(movedCC_, otherCC.get());
                }
            } else {
                collidables.erase(i);
                i--;
            }
        }   
    }
} 

void CollisionSystem::handleCollision(CollisionComponent* ccone, CollisionComponent* cctwo) {
    if (handleCollisionOneAxis(ccone->getAxisBounds(X), cctwo->getAxisBounds(X)) &&
        handleCollisionOneAxis(ccone->getAxisBounds(Y), cctwo->getAxisBounds(Y)) &&
        handleCollisionOneAxis(ccone->getAxisBounds(Z), cctwo->getAxisBounds(Z))) {
        ccone->onCollision(cctwo);
        cctwo->onCollision(ccone);
    } 
}
