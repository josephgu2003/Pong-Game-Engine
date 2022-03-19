//
//  Painter.cpp
//  Pong
//
//  Created by Joseph Gu on 3/17/22.
//

#include "Painter.hpp"
#include "Actor.hpp"
#include "World.hpp"
#include "PaintingSubject.hpp"
#include "NotificationSystem.hpp"
#include "AnimComponent.hpp"
#include "InputHandler.hpp"

Painter::Painter(Componentable& compUser_, Actor& actor) : Component(compUser_),
actorRef(actor) {
    targetedSubject = nullptr;
    paintInProgress = false;
    recentPainting = "";
}

void Painter::tick() {
    World& w = actorRef.getWorld();
    float distance = 100.0f;
    auto nearestSubj = w.getNearestObjectWith<PaintingSubject>(&actorRef, distance);
    
    if (distance < 5.0f) {
        if (targetedSubject != nearestSubj) { actorRef.getWorld().getComponent<NotificationSystem>()->newNotification("Press T to paint the Subject", 3.0f);
            targetedSubject = nearestSubj;
        }
    } else {
        targetedSubject = nullptr;
    }
     
    if (paintInProgress) {
        if (paintTime.getTime() > 20.0f && targetedSubject) {
            recentPainting = targetedSubject->getComponent<PaintingSubject>()->getName();
            actorRef.getWorld().getComponent<NotificationSystem>()->newNotification("Your comprehension of " + recentPainting + " has increased", 3.0f);
            paintInProgress = false; 
            actorRef.getComponent<AnimComponent>()->playDefault();
        } 
    }
}
   
void Painter::notify(const Subject &subject, GameEvent ge) {
    if (ge == KEY_PRESSED && targetedSubject != nullptr) {
        if (static_cast<const InputHandler&>(subject).getCurrentKeyPress() == GLFW_KEY_T) {
            actorRef.getComponent<AnimComponent>()->playAnim("Painting", 40, 75);
            paintTime.resetTime();
            paintInProgress = true; 
        }
    }
}

std::string Painter::getMostRecentPainting() const {
    return recentPainting;
}
