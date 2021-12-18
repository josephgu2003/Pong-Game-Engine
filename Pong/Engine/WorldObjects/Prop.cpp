//
//  Prop.cpp
//  Pong
//
//  Created by Joseph Gu on 11/11/21.
//

#include "Prop.hpp"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GraphicsComponent.hpp"

Prop::Prop() : Componentable(), Positionable() {
    
}
 
void Prop::tick() {
    Componentable::tick();
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, posVec);
    glm::vec3 rotations = glm::vec3(eulerAngles.x,glm::radians(eulerAngles.y),glm::radians(eulerAngles.z));
    glm::quat MyQuaternion = glm::quat(rotations); 
    
    glm::mat4 RotationMatrix = toMat4(MyQuaternion);
    modelMat = modelMat * RotationMatrix;
    
    GraphicsComponent* graphics = this->getComponent<GraphicsComponent>();
     
    graphics->getShader()->setMat4("modelMat", modelMat);
    
    glm::mat3 transposeInverse = glm::mat3(glm::transpose(glm::inverse(modelMat)));
    graphics->getShader()->setMat3("transposeInverseModelMat", transposeInverse);
}

void Prop::setWorld(World* world_) {
    world = world_;
}
World& Prop::getWorld() {
    return *world;
}
 
// want the prop to have copies in multiple places
// derived class or component??
