//
//  MovementController.cpp
//  Pong
//
//  Created by Joseph Gu on 2/28/22.
//

#include "MovementController.hpp"
#include "Actor.hpp"
#include "AnimComponent.hpp"

MovementController::MovementController(Actor& a) : Component(a) {
    moveMode = MOVEMODE_IDLE;
}
 
void MovementController::move(MovementDirection dir) {
    Actor* a = static_cast<Actor*>(actor);
    
    auto walkInDirection = [&] (float speed) {
        a->posDir(speed);
        if (auto anim = a->getComponent<AnimComponent>()) {
            anim->playAnimIfNotPlaying("Walk");
        }
    };
    switch (dir) {
        case MOVEDIR_FORWARDS: {
            walkInDirection(walkSpeed);
            break;
        }
            
        case MOVEDIR_BACKWARDS: {
            walkInDirection(-walkSpeed);
            break;
        }
            
        case MOVEDIR_RIGHT: {
            if (moveMode == MOVEMODE_IDLE) {
                if (auto cam = a->getCamera()) { 
                    a->orientYawTo(-cam->getRight());
                    cam->setStateAndInterpolate(CAM_FOLLOW_ACTOR_UNALIGNED, 2.0f);
                } else {
                    a->orientYawTo(-a->getRight());
                }
            } 
            walkInDirection(walkSpeed);
            break;
        }
        case MOVEDIR_LEFT: {
            if (moveMode == MOVEMODE_IDLE) {
                if (auto cam = a->getCamera()) {
                    a->orientYawTo((cam->getRight()));
                    cam->setStateAndInterpolate(CAM_FOLLOW_ACTOR_UNALIGNED, 2.0f);
                } else {
                    a->orientYawTo((a->getRight()));
                } 
            }
            walkInDirection(walkSpeed);
            break;
        }
            
    }
    moveMode = MOVEMODE_WALK;
}

void MovementController::makeIdle() {
    moveMode = MOVEMODE_IDLE;
    Actor* a = static_cast<Actor*>(actor); 
    
    if (auto cam = a->getCamera()) {
        cam->setStateAndInterpolate(CAM_FOLLOW_ACTOR_ALIGNED, 2.0f);
    }
    
    if (auto anim = a->getComponent<AnimComponent>()) {
        anim->playDefault();
    }
}

void MovementController::tick() {
    
}
 
