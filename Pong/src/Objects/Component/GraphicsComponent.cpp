//
//  GraphicsComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 6/27/21.
//

#include "GraphicsComponent.hpp"

GraphicsComponent::GraphicsComponent() {
    
}

GraphicsComponent::GraphicsComponent(VertexData* vertexData_, Shader* shader_) {
    vertexData.reset(vertexData_);
    shader = shader_;
}

void GraphicsComponent::init(VertexData* vertexData_, Shader* shader_) {
    vertexData.reset(vertexData_);
    shader = shader_;
}


VertexData* GraphicsComponent::getVertexData() {
    return vertexData.get();
}


void GraphicsComponent::tick(Actor &actor, World &world) {
    
}


Shader* GraphicsComponent::getShader() {
    return shader;
}
