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
#include "Force.hpp"
class Game;

class Fish: public Ability {
    Quad quad;
    Force* force= NULL;
    bool mode2 = false;
    glm::vec3 dirFired;
    void freeActor();
    int step = 0;
    Game* game = NULL; 
public:
    Fish(World* world_, Actor* actor_, float duration_);
    ~Fish();
    virtual void call();
    virtual void tick();
};

#endif /* Fish_hpp */
