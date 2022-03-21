//
//  Firestarter.cpp
//  Pong
//
//  Created by Joseph Gu on 3/20/22.
//

#include "Firestarter.hpp"
#include "Actor.hpp"
#include "World.hpp"
#include "NotificationSystem.hpp"
#include "AnimComponent.hpp"
#include "InputHandler.hpp"
#include "ParticleFactory.hpp"

FireStarter::FireStarter(Componentable& compUser_, Actor& actor) : Component(compUser_),
actorRef(actor) {

}

void FireStarter::tick() {
    if (actorRef.getComponent<AnimComponent>()->getCurrentAnimation() != "Painting") {
        lightingTime.cancelCountdown();
    }
    
    if (lightingTime.checkDone()) {
        ParticleFactory pf;
        auto fire = pf.makeParticles(PE_BODYSPARKS, actorRef.getPos() + actorRef.getDir());
        actorRef.getWorld().insert(fire);
    }
}
   
void FireStarter::notify(const Subject &subject, GameEvent ge) {
    if (ge == KEY_PRESSED) {
        if (static_cast<const InputHandler&>(subject).getCurrentKeyPress() == GLFW_KEY_F) {
            actorRef.getComponent<AnimComponent>()->playAnim("Painting", 41, 75);
            lightingTime.windGentle(10.0f);
        }
    }
}
 
