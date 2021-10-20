//
//  GraphicsComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 6/27/21.
//

#include "GraphicsComponent.hpp"
#include "Renderer.hpp"

GraphicsComponent::GraphicsComponent() : Renderable::Renderable() {
    Component::type = GRAPHICS;
}
 
void GraphicsComponent::tick() {
    
}

void GraphicsComponent::init(Shader* shader_, const std::string& model, const TextureMaps& map_) {
    shader = shader_;
    shader->use();
    // need to load model data to vao vbo,
    VertexLoader::loadModel(model, VAO, VBO, EBO, numIndices); 
    map = map_;
}

void GraphicsComponent::animateTexture(Texture* texture, Shader* shader_) {
    FrameAndShader fas = {nullptr, nullptr};
    fas.shader = shader_;
    AssetManager::generateFramebuffer(fas.frame, &texture->id, texture->dimensions.x, texture->dimensions.y);
}

void GraphicsComponent::draw(Renderer* r) {
    r->renderElements(this);
} 
  
  
