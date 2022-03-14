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
    if (moveMode == MOVEMODE_PARALYZED) {
        return;
    }
    
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
                 //   cam->setState(CAM_FOLLOW_ACTOR_UNALIGNED);
                        cam->setAlignmentToActorDir(glm::vec3(0.0f, 1.0f, 0.0f),
                                                    90.0f);
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
                    //cam->setState(CAM_FOLLOW_ACTOR_UNALIGNED);
                    cam->setAlignmentToActorDir(glm::vec3(0.0f, 1.0f, 0.0f),
                                                -90.0f);
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
    if (moveMode == MOVEMODE_PARALYZED) {
        return; 
    }
    moveMode = MOVEMODE_IDLE;
    Actor* a = static_cast<Actor*>(actor); 
    
    if (auto cam = a->getCamera()) {
        a->orientYawTo((cam->getDir()));
      //  cam->setState(CAM_FOLLOW_ACTOR_ALIGNED);
        cam->setAlignmentToActorDir(glm::vec3(0.0f, 1.0f, 0.0f),
                                    0.0f);
    }
    
    if (auto anim = a->getComponent<AnimComponent>()) {
        anim->playDefault();
    }
} 

void MovementController::tick() {
    if (timeLeftOnParalysis <= 0.0f && moveMode == MOVEMODE_PARALYZED) {
        moveMode = MOVEMODE_IDLE;
    } else {
        timeLeftOnParalysis -= watch.getTime();
        watch.resetTime();
    }
}
 
void MovementController::setMovementMode(MovementMode mm) {
    moveMode = mm;
}

void MovementController::paralyze(float duration) {
    moveMode = MOVEMODE_PARALYZED;
    watch.resetTime();
    timeLeftOnParalysis = duration;
}
