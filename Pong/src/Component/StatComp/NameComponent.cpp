//
//  NameComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 9/9/21.
//

#include "NameComponent.hpp"

NameComponent::NameComponent(Actor& actor) : Component(actor) {
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
 
