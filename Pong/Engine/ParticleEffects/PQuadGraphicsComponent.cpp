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

PQuadGraphicsComponent::PQuadGraphicsComponent(ParticleSystem& pe, int numberParticles, float size, Shader* shader, const TextureMaps& map) : PGraphicsComponent(pe, size, shader, map){
    int datasize = numberParticles * 17 * sizeof(float);
    std::vector<int> v = {4,4,4,4,1};
    VertexLoader::load2DQuadData(VAO, VBO, EBO, numIndices, glm::vec2(size, size), glm::vec2(0,0));
    makeInstanceBuffer(datasize, 2, v, actor->getNumParticles());
    drawTarget = GL_TRIANGLES; 
} 

void PQuadGraphicsComponent::tick() {
    std::vector<float> v;
    int n = actor->getNumParticles();
    Particle* p = actor->getParticles();
    v.resize(n*17);
    
    glm::mat4 modelMatr = glm::mat4(1.0f);
    
    int counter = 0;
    for (int i = 0; i < n; i++) {
        Particle& particle = p[i];
        modelMatr = glm::mat4(1.0f);
        modelMatr = glm::translate(modelMatr, particle.posVec);
        modelMatr = glm::rotate(modelMatr, -glm::radians(particle.pyrAngles.y), glm::vec3(0,1,0));
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
