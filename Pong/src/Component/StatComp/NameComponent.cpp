//
//  NameComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 9/9/21.
//

#include "NameComponent.hpp"
#include "Actor.hpp"
#include "World.hpp"

NameComponent::NameComponent(Actor& actor) : ActorComp(actor) {
    Component::type = NAME;
}

void NameComponent::init(const std::string& common, const std::string& id) {
    commonName = common;
    idName = id;
}

const std::string& NameComponent::getCommonName() {
    return commonName;
}

const std::string& NameComponent::getIdName() {
    return idName;
}

void NameComponent::tick() {
    
}

void NameComponent::speak(const std::string& speech, float duration) {
    std::string s = std::string(commonName + ": " + speech);
    actor->getWorld().newSoundText(s, actor->getPos(), duration); 
} 
  
