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

PGraphicsComponent::PGraphicsComponent(ParticleSystem& pe, float size, Shader* shader_, const TextureMaps& map_) : ParticleComp(pe), Renderable(){
    shader = shader_;
    shader->use();
    shader->setFloat("size", size);
    extern GLuint uboViewProj;
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "ViewProj"), 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewProj);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // need to load model data to vao vbo,
    map = map_;
    type = GRAPHICS;
}


void PGraphicsComponent::draw(Renderer* r) {
    r->renderParticles(this, actor->getNumParticles());
} 
      
void PGraphicsComponent::tick() {
    
}

void PGraphicsComponent::setColor(float r, float g, float b) {
    shader->use();
    shader->setVec4("color", glm::vec4(r,g,b, 1.0));
}
