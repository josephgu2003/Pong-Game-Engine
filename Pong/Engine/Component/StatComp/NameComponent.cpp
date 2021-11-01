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
    name = "";
}

const std::string& NameComponent::getName() {
    return name;
}

void NameComponent::init(const std::string& name_) {
    name = name_;
}

void NameComponent::tick() {
    
}

void NameComponent::speak(const std::string& speech, float duration) {
    std::string s = std::string(name + ": " + speech);
    actor->getWorld().newSoundText(s, actor->getPos(), duration); 
} 
  
