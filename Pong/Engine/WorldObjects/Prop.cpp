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

    GraphicsComponent* graphics = this->getComponent<GraphicsComponent>();
     
    graphics->getShader()->setUniform("modelMat", getModelMatrix());
    
    glm::mat3 transposeInverse = glm::mat3(glm::transpose(glm::inverse(getModelMatrix())));
    graphics->getShader()->setUniform("transposeInverseModelMat", transposeInverse);
} 

void Prop::setWorld(World* world_) {
    world = world_;
}
World& Prop::getWorld() {
    return *world;
}
 
// want the prop to have copies in multiple places
// derived class or component??
