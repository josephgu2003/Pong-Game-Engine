//
//  Renderable.cpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#include "GraphicsObject.hpp"
#include "Shader.hpp"
#include "VertexLoader.hpp"

int GraphicsObject::nonHolders = 0;
DrawPass GraphicsObject::getDrawPass() {
    return drawPass;
}


GraphicsObject::GraphicsObject(DrawPass dp) {
    drawPass = dp;
    glGenVertexArrays(1, &VAO);
    
    glGenBuffers(1, &VBO);
    
    glGenBuffers(1, &EBO);
     
    glGenBuffers(1, &instanceVBO);
    
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


GraphicsObject::~GraphicsObject() {
    if (!movedFrom && deleteDataOnDestruct) {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &instanceVBO);
        glDeleteVertexArrays(1, &VAO);
        if(shader) {
            delete shader;
        }
    } else {
        nonHolders--;
    } 
}

bool GraphicsObject::isInstanced() {
    if (instanceCount > 1) {
        return true;
    }
    return false;
}

void GraphicsObject::setTextureAnimation(std::string frameIndexUniform, int numFrames, float fps) {
    textureAnimations.emplace_back(frameIndexUniform, numFrames,fps);
}

TextureAnimation::TextureAnimation(const std::string& name_, int maxFrames_, float fps_) {
    uniformName = name_;
    maxFrames = maxFrames_;
    timePerFrame = 1.0f / fps_;
    watch.resetTime();
    currentFrame = 0;
} 

void GraphicsObject::animateTextures() {
    if (textureAnimations.size() > 0) {
        shader->use();
    } else {
        return;
    }
    for (auto it = textureAnimations.begin(); it != textureAnimations.end(); it++) {
        float frameNow = (*it).watch.getTime()/(*it).timePerFrame;
        (*it).currentFrame = std::fmod(frameNow,(*it).maxFrames);
        shader->setUniform((*it).uniformName, (*it).currentFrame);
    }
}

GLuint GraphicsObject::getInstanceCount() {
    return instanceCount;
}

void GraphicsObject::setSingularMaterial(const Material& map_) {
    map = map_;
}

Material& GraphicsObject::getSingularMaterial() {
    return map;
}
