//
//  Renderable.cpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#include "GraphicsObject.hpp"
#include "Shader.hpp"
#include "VertexLoader.hpp"

GraphicsObject::GraphicsObject() {
    glGenVertexArrays(1, &VAO);
    
    glGenBuffers(1, &VBO);
     
    glGenBuffers(1, &EBO);
    
    drawTarget = GL_TRIANGLES;
    
    shader = new Shader();
    numIndices = 0;
    instanceCount = 1;
}
 
GLenum GraphicsObject::getDrawTarget() {
    return drawTarget;
}

Shader* GraphicsObject::getShader() {
    return shader;
}
 
void GraphicsObject::setShader(Shader* shader_) {
    shader = shader_; 
}
 
Material& GraphicsObject::getTextureMap() {
    return map;
}

void GraphicsObject::bind() {
    glBindVertexArray(VAO);
}
void GraphicsObject::unbind() { 
    glBindVertexArray(0);
}

 
GLuint GraphicsObject::getNumIndices() {
    return numIndices; 
}

void GraphicsObject::makeInstanceBuffer(int dataSize_, int firstAttribLocation, const std::vector<int>& layout, unsigned int instanceCount_) {
    instanceCount = instanceCount_;
   glBindVertexArray(VAO);
   glGenBuffers(1,  &instanceVBO);
   glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
   glBufferData(GL_ARRAY_BUFFER, dataSize_, nullptr, GL_DYNAMIC_DRAW);
   VertexLoader::setupVAOAttribsInstancing(firstAttribLocation, layout);
   glBindVertexArray(0);
}

void GraphicsObject::updateInstanceBuffer(const std::vector<float>& vec) {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vec.size()*sizeof(float), (void*) vec.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
 
   
