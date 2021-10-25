//
//  Renderable.cpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#include "Renderable.hpp"
#include "Shader.hpp"
#include "VertexLoader.hpp"

Renderable::Renderable() {
    glGenVertexArrays(1, &VAO);
    
    glGenBuffers(1, &VBO);
     
    glGenBuffers(1, &EBO);
    
    drawTarget = GL_TRIANGLES;
    
    shader = new Shader();
    numIndices = 0;
    instanceCount = 1;
}
 
GLenum Renderable::getDrawTarget() {
    return drawTarget;
}

Shader* Renderable::getShader() {
    return shader;
}
 
void Renderable::setShader(Shader* shader_) {
    shader = shader_; 
}
 
TextureMaps& Renderable::getTextureMap() {
    return map;
}

void Renderable::bind() {
    glBindVertexArray(VAO);
}
void Renderable::unbind() { 
    glBindVertexArray(0);
}

 
GLuint Renderable::getNumIndices() {
    return numIndices; 
}

void Renderable::makeInstanceBuffer(int dataSize_, int firstAttribLocation, const std::vector<int>& layout, unsigned int instanceCount_) {
    instanceCount = instanceCount_;
   glBindVertexArray(VAO);
   glGenBuffers(1,  &instanceVBO);
   glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
   glBufferData(GL_ARRAY_BUFFER, dataSize_, nullptr, GL_DYNAMIC_DRAW);
   VertexLoader::setupVAOAttribsInstancing(firstAttribLocation, layout);
   glBindVertexArray(0);
}

void Renderable::updateInstanceBuffer(const std::vector<float>& vec) {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vec.size()*sizeof(float), (void*) vec.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
 
   
