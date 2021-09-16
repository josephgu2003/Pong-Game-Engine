//
//  GraphicsComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 6/27/21.
//

#include "GraphicsComponent.hpp"
GraphicsComponent::GraphicsComponent() {
    Component::type = GRAPHICS;
}

GraphicsComponent::GraphicsComponent(VertexData* vertexData_, Shader* shader_) {
    Component::type = GRAPHICS; 
    vertexData.reset(vertexData_);
    shader = shader_;
}

void GraphicsComponent::setModel(Model* model_) {
    model = model_;
}

void GraphicsComponent::init( VertexData* vertexData_, Shader* shader_) {
    vertexData.reset(vertexData_);
    shader = shader_;
}

VertexData* GraphicsComponent::getVertexData() {
    return vertexData.get();
}

void GraphicsComponent::tick() {
    
}

Shader* GraphicsComponent::getShader() {
    return shader;
}

void GraphicsComponent::animateTexture(Texture* texture, Shader* shader_) {
    FrameAndShader fas = {nullptr, nullptr};
    fas.shader = shader_;
    AssetManager::generateFramebuffer(fas.frame, &texture->id, texture->dimensions.x, texture->dimensions.y);
}

Model* GraphicsComponent::getModel() {
    return model;
}
