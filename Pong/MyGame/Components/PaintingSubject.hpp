//
//  PaintingSubject.hpp
//  Pong
//
//  Created by Joseph Gu on 3/17/22.
//

#ifndef PaintingSubject_hpp
#define PaintingSubject_hpp

#include <stdio.h>
#include "Component.hpp"
#include "Particle.hpp"
#include <string>

class Prop;
class World;

class PaintingSubject : public Component {
    Prop& propRef;
    World& worldRef;
    std::weak_ptr<ParticleSystem> sparklesRef;
    std::string name;
public:
    PaintingSubject(Componentable& compUser_, Prop& prop, World& world, const std::string& name_);
    
    void tick() override;
    
    std::string getName();
};

#endif /* PaintingSubject_hpp */
