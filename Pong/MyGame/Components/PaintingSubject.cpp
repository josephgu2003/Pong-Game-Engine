//
//  PaintingSubject.cpp
//  Pong
//
//  Created by Joseph Gu on 3/17/22.
//

#include "PaintingSubject.hpp"
#include "ParticleFactory.hpp" 
#include "Prop.hpp" 
#include "World.hpp"

PaintingSubject::PaintingSubject(Componentable& compUser_, Prop& prop, World& world, const std::string& name_) : Component(compUser_), propRef(prop), worldRef(world) {
    ParticleFactory pf;
    auto sparkles = pf.makeParticles(PE_RUNICLETTERS, prop.getPos());
    sparklesRef = sparkles;
    world.insert(sparkles);
    name = name_;
}

void PaintingSubject::tick() {
  
}
 
std::string PaintingSubject::getName() {
    return name;
}
