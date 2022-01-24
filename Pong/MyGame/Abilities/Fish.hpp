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
#include "MyActorFactory.hpp"
class Game;

class Fish: public Ability {
//    Quad quad;
    MyActorFactory factory;
    std::shared_ptr<Actor> fish;
    bool mode2 = false;
    glm::vec3 dirFired;
    void freeActor();
    int step = 0;
    Game* game = NULL; 
public:
    Fish(World* world_, Actor* actor_, float duration_);
    ~Fish();
    void call() override;
    void tick() override;
};

#endif /* Fish_hpp */
