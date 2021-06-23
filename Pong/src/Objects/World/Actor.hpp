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


#include "Shader.hpp"
//something that can move and has a model

class World;
class Ability;
class Shader;

struct Light {
    bool exists;
    float intensity;
};

enum State {
    STATE_NORMAL,
    STATE_PARALYZED,
    STATE_FLYING
};

class Actor {
    friend class Camera;
    friend class Renderer;
    //float vertices [24] = {0.0};
   // GLuint indices [36] = {0};
protected:
    Model* model;
    std::vector<Component*> components;
    Shader shader;
public:
    std::vector<Ability*> abilityQ;
    glm::vec3 posVec;
    glm::vec3 dirVec;
    glm::vec3 eulerAngles;
    float height = 0.0f;
    glm::vec3 velVec;
    glm::vec3 upVec;
    glm::vec3 rightVec;
    World* world;
    glm::vec3 camFocus = glm::vec3(0,0.5,0);
    State state;
    
    Actor();
    ~Actor();

    virtual void tick();
    void init();
    void setWorld(World* world_);
    
    float getYaw();
    glm::vec3 getPos();
    
    void orient(float yaw);
    void turnTowards(glm::vec3 newDir);
    void posDir(float speed);
    void posRight(float speed);
    void jump();
    void velRight(float speed);
    void velDir(float speed);
};

#endif /* Actor_hpp */
