//
//  NameComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 9/9/21.
//

#include "NameComponent.hpp"
#include "Actor.hpp"
#include "World.hpp"
#include "SubtitlesSystem.hpp"

NameComponent::NameComponent(Actor& actor) : Component(actor) {
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
    static_cast<Actor*>(actor)->getWorld().getComponent<SubtitlesSystem>()->newSoundText(s, duration);
} 
    
  
 
