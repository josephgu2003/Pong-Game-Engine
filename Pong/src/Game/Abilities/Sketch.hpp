//
//  Sketch.hpp
//  Pong
//
//  Created by Joseph Gu on 6/8/21.
//

#ifndef Sketch_hpp
#define Sketch_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "Ability.hpp"
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "World.hpp"

class Sketch: public Ability {
    Quad quad;
    bool mode2 = false;
    glm::vec3 dirFired;
public:
    Sketch(World* world_, Actor* actor_, float duration_, GLuint texture_);
    ~Sketch();
    virtual void call(void* pointer);
    virtual void tick();
    void call2();
};
#endif /* Sketch_hpp */
