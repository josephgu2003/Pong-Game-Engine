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
#include <random>
#include <algorithm>
#include "Shader.hpp"
#include "Numberable.hpp"
#include "GraphicsComponent.hpp"
#include <memory>
#include "VertexData.hpp"
#include "Componentable.hpp"
//something that can move and has a model

class World;
class Ability;
class Shader;


enum State {
    STATE_NORMAL,
    STATE_PARALYZED,
    STATE_FLYING
};

class Actor : public Numberable, public Componentable {
    friend class Camera;
    friend class Renderer;
protected:
    World* world = NULL;
    std::shared_ptr<GraphicsComponent> graphics = NULL;
    glm::mat4 modelMat = glm::mat4(1.0f);
    State state;
    glm::vec3 posVec;
    glm::vec3 velVec;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;
public:

    
    glm::vec3 dirVec;
    glm::vec3 eulerAngles;
    float height = 0.0f;
    glm::vec3 upVec;
    glm::vec3 rightVec;
    glm::vec3 camFocus = glm::vec3(0,0.5,0);
    
    Actor();
    ~Actor();
     
    void addComp(const std::shared_ptr<Component>& comp) override;
    
    virtual void tick();
    virtual void init(int i);
    void setWorld(World* world_);
    World& getWorld();
    
    float getYaw();
    
    glm::vec3 getPos();
    void setPos(glm::vec3 pos_);
    void translatePos(glm::vec3 translate);
    void setPosY(float y_);
    void orient(float yaw);
    void turnTowards(glm::vec3 newDir);
    void posDir(float speed);
    void posRight(float speed);
    void randomPosAround(glm::vec3 pivot);
    
    void jump();
    void setVel(glm::vec3 velVel_);
    void velRight(float speed);
    void velDir(float speed);
    glm::vec3 getVel();
    void accelerate(glm::vec3 accel);

    void setState(State state);
    State getState();
};

#endif /* Actor_hpp */
