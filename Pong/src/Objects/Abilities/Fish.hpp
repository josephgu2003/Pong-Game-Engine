//
//  Fish.hpp
//  Pong
//
//  Created by Joseph Gu on 6/22/21.
//

#ifndef Fish_hpp
#define Fish_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "Ability.hpp"
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "World.hpp"

class Game;

class Fish: public Ability {
    Quad quad;
    bool mode2 = false;
    glm::vec3 dirFired;
public:
    Fish(World* world_, Actor* actor_, float duration_);
    ~Fish();
    virtual void call(Game* game);
    virtual void tick();
};

#endif /* Fish_hpp */
