//
//  PGraphicsComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 10/20/21.
//

#include "PGraphicsComponent.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include <glm/glm.hpp>


PGraphicsComponent::PGraphicsComponent(ParticleSystem& pe, float size, Shader* shader_, const Material& map_) : Component(pe), GraphicsObject(DRAW_TRANSPARENT){
    shader = shader_; 
    shader->use();
    shader->setUniform("size", size); 
    Renderer::bindShaderUniblock(shader, ViewProj); 
    // need to load model data to vao vbo,
    setSingularMaterial(map_);
    updatePriority = 10;
} 

PGraphicsComponent::~PGraphicsComponent() { 
    
}
void PGraphicsComponent::draw(Renderer* r) {
    r->renderParticles(this);
}  
      
void PGraphicsComponent::tick() { 
    animateTextures();
}

void PGraphicsComponent::setColor(float r, float g, float b) {
    shader->use();
    shader->setUniform("color", glm::vec4(r,g,b, 1.0));
}
