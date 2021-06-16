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
#include "Model.hpp"
#include <vector>
#include "Component.hpp"

//something that can move and has a model

class World;
class Ability;

struct Light {
    bool exists;
    float intensity;
};

class Actor {
    friend class Camera;
    friend class Renderer;
    //float vertices [24] = {0.0};
   // GLuint indices [36] = {0};
protected:
    Model* model;
    float yaw;
    float height = 0.0f;
   // glm::vec3 posVec;
    glm::vec3 velVec;
    glm::vec3 upVec;
    glm::vec3 rightVec;
    World* world;
    glm::vec3 camFocus = glm::vec3(0,0.5,0);
    std::vector<Component*> components;
public:
    int k = 2;
    std::vector<Ability*> abilityQ;
    glm::vec3 posVec;
    glm::vec3 dirVec;
    Actor();
    ~Actor();
    void updateVecs();
    void orient(float yaw);
    virtual void tick();
    void stop();
    void posDir(float speed);
    void posRight(float speed);
    void jump();
    void velRight(float speed);
    void velDir(float speed);
    float getYaw();
    glm::vec3 getPos();
    void loadModel();
    void setWorld(World* world_);
};

#endif /* Actor_hpp */
