//
//  PQuadGraphicsComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 10/21/21.
//

#include "PQuadGraphicsComponent.hpp"
#include "VertexLoader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Particle.hpp"
#include "Renderer.hpp"

PQuadGraphicsComponent::PQuadGraphicsComponent(ParticleSystem& pe, std::string modelFile, int numberParticles, float size_, Shader* shader, const Material& map) : GraphicsComponent(pe, shader, map, DRAW_TRANSPARENT) {
    
    shader->use();
    shader->setUniform("size", size_); 
    Renderer::bindShaderUniblock(shader, ViewProj);
    // need to load model data to vao vbo,
    type = GRAPHICS;
    
    drawCall = [] (Renderer* r, GraphicsComponent* gc) {
        r->renderParticles(gc);
    };
    
    size = size_;
    int datasize = numberParticles * 17 * sizeof(float);
    std::vector<int> v = {4,4,4,4,1};
    VertexLoader::loadModelSimple(modelFile, VAO, VBO, EBO, numIndices);
    makeInstanceBuffer(datasize, 3, v, static_cast<ParticleSystem*>(actor)->getNumParticles());
    drawTarget = GL_TRIANGLES;
}

PQuadGraphicsComponent::~PQuadGraphicsComponent() {
     
}

PQuadGraphicsComponent::PQuadGraphicsComponent(ParticleSystem& pe, int numberParticles, float size_, Shader* shader, const Material& map) : GraphicsComponent(pe, shader, map, DRAW_TRANSPARENT) {
    
    shader->use();
    shader->setUniform("size", size_);
    Renderer::bindShaderUniblock(shader, ViewProj);
    // need to load model data to vao vbo,
    type = GRAPHICS;
    
    drawCall = [] (Renderer* r, GraphicsComponent* gc) {
        r->renderParticles(gc);
    };
    
    int datasize = numberParticles * 17 * sizeof(float);
    size = size_;
    std::vector<int> v = {4,4,4,4,1};
    VertexLoader::load2DQuadData(VAO, VBO, EBO, numIndices, glm::vec2(1.0, 1.0), glm::vec2(0,0));
    makeInstanceBuffer(datasize, 3, v, static_cast<ParticleSystem*>(actor)->getNumParticles());
    drawTarget = GL_TRIANGLES;
}  

void PQuadGraphicsComponent::tick() {
    GraphicsComponent::tick();
    std::vector<float> v;
    int n = static_cast<ParticleSystem*>(actor)->getNumParticles();
    Particle* p = static_cast<ParticleSystem*>(actor)->getParticles();
    v.resize(n*17);
    
    glm::mat4 modelMatr = glm::mat4(1.0f);
    
    int counter = 0;
    for (int i = 0; i < n; i++) {
        Particle& particle = p[i];
        modelMatr = glm::mat4(1.0f); 
        modelMatr = glm::translate(modelMatr, particle.posVec);
        modelMatr = glm::rotate(modelMatr, -glm::radians(particle.pyrAngles.y), glm::vec3(0,1,0));
        modelMatr = glm::scale(modelMatr, glm::vec3(size,size,size));
        float* mat = glm::value_ptr(modelMatr[0]);
            for (int l = 0; l < 16; l++) {
                v[counter] = (mat[l]);
                counter++; 
            }
        v[counter] = particle.duration;
        counter++;
    }//may need optimization contigious vs separated memory
    updateInstanceBuffer(v);
}
