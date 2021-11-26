//
//  GraphicsComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 6/27/21.
//

#include "GraphicsComponent.hpp"
#include "Renderer.hpp"
#include "Shader.hpp" 
GraphicsComponent::GraphicsComponent(Componentable& actor, Shader* shader_, const Material& map_) : Component(actor), GraphicsObject::GraphicsObject() {
    Component::type = GRAPHICS; 
    shader = shader_;
    map = map_;
    drawCall = [] (Renderer* r, GraphicsComponent* gc) {
        r->renderActor(gc);
    };
}
 
void GraphicsComponent::tick() {
    if (mesh) {
        updateVertexBuffer<SimpleVertex>(mesh); 
    }
}

void GraphicsComponent::initModel(const std::string& model) {
    // need to load model data to vao vbo,
    VertexLoader::loadModel(model, VAO, VBO, EBO, numIndices);
}

void GraphicsComponent::animateTexture(Texture* texture, Shader* shader_) {
    FrameAndShader fas = {nullptr, nullptr};
    fas.shader = shader_;
    AssetManager::generateFramebuffer(fas.frame, &texture->id, texture->dimensions.x, texture->dimensions.y);
}

void GraphicsComponent::draw(Renderer* r) {
    drawCall(r, this);
} 
   
void GraphicsComponent::initGrid(int verticesX, int verticesY, float scale, VertexMesh*& mesh_) {
    VertexLoader::loadSimpleVertexGrid(verticesX, verticesY, scale, mesh_->vertices, VAO, VBO, EBO, numIndices);
    mesh = mesh_;
}

void GraphicsComponent::setDrawCall(DrawCall dc) {
    drawCall = dc;
}
