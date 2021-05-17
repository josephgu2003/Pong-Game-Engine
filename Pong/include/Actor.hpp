//
//  Actor.hpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#ifndef Actor_hpp
#define Actor_hpp

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <string>
#include "Model.hpp"
#include <iostream>

class Actor {
    friend class Camera;
    friend class Renderer;
    float vertices [24] = {0.0};
    GLuint indices [36] = {0};
protected:
    Model* model;
    float yaw;
    float height = 0.0f;
    glm::vec3 posVec;
    glm::vec3 velVec;
    glm::vec3 dirVec;
    glm::vec3 upVec;
    glm::vec3 rightVec;
public:
    Actor();
    ~Actor();
    void updateVecs();
    void orient(float yaw);
    void stop();
    void posDir(float speed);
    void posRight(float speed);
    void jump();
    void velRight(float speed);
    void velDir(float speed);
    float getYaw();
    float* getVertices();
    GLuint* getIndices();
    glm::vec3 getPos();
    int getVerticesCount();
    int getIndicesCount();
    void loadModel();
};
#endif /* Actor_hpp */
