//
//  FriendAI.cpp
//  Pong
//
//  Created by Joseph Gu on 9/29/21.
//

#include "FriendAI.hpp"
#include "Actor.hpp"

FriendAI::FriendAI(Actor& actor, Actor& friendly_) : AIComponent(actor){
    friendly = &friendly_;
}

void FriendAI::tick() {
    
}

void FriendAI::accompanyFriend() {
    if (actor->getDistanceTo(friendly) > 20.0) {
        actor->jump();
    }
}
