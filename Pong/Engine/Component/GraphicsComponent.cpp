//
//  GraphicsComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 6/27/21.
//

#include "GraphicsComponent.hpp"
#include "Renderer.hpp"
#include "VertexLoader.hpp" 
#include "Shader.hpp" 
GraphicsComponent::GraphicsComponent(Componentable& actor, Shader* shader_, const Material& map_, DrawPass dp) : Component(actor), GraphicsObject::GraphicsObject(dp) {
    Component::type = GRAPHICS; 
    shader = shader_;
    setSingularMaterial(map_);
    drawCall = [] (Renderer* r, GraphicsComponent* gc) {
        r->renderGeneric(gc);
    };
}

GraphicsComponent::~GraphicsComponent() {
    
}

void GraphicsComponent::tick() {
    if (auto x = mesh.lock()) {
        updateVertexBuffer<SimpleVertex>(x.get());
    }
    animateTextures();
}
 
void GraphicsComponent::initModel(const std::string& model) {
    // need to load model data to vao vbo, 
    VertexLoader::loadModel(model, VAO, VBO, EBO, numIndices);
}

void GraphicsComponent::draw(Renderer* r) {
    drawCall(r, this);
} 
    
void GraphicsComponent::initGrid(int verticesX, int verticesY, float scale, std::shared_ptr<VertexMesh>& mesh_) {
    VertexLoader::loadSimpleVertexGrid(verticesX, verticesY, scale, mesh_->vertices, VAO, VBO, EBO, numIndices);
    mesh = mesh_;
}
 
void GraphicsComponent::setDrawCall(DrawCall dc) {
    drawCall = dc;
}
 
void GraphicsComponent::setColor(float r, float g, float b) {
    shader->use();
    shader->setUniform("color", glm::vec4(r,g,b,1.0f));  
}
