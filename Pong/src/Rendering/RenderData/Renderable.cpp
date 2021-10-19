//
//  Renderable.cpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#include "Renderable.hpp"
#include "Shader.hpp"
 
Renderable::Renderable() {
    glGenVertexArrays(1, &VAO);
    
    glGenBuffers(1, &VBO);
     
    glGenBuffers(1, &EBO);
    
    numIndices = 0;
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
