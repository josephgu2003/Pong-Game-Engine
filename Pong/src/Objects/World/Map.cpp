//
//  Map.cpp
//  Pong
//
//  Created by Joseph Gu on 6/24/21.
//

#include "Map.hpp"
#include <vector>
#include "AssetManager.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

void Map::init() {
    shader.init("Shaders/MapVertexShader.vs", "Shaders/MapFragmentShader.fs");
    Vertex a,b,c,d;
    a = {glm::vec3(-100,0,-100),glm::vec3(0,1,0),glm::vec2(0,0)};
    b = {glm::vec3(100,0,-100),glm::vec3(0,1,0),glm::vec2(1,0)};
    c = {glm::vec3(100,0,100),glm::vec3(0,1,0),glm::vec2(1,1)};
    d = {glm::vec3(-100,0,100),glm::vec3(0,1,0),glm::vec2(0,1)};
    std::vector<Vertex> mapVertices = {a,b,c,d
    };
    
    std::vector<GLuint> mapIndices = {
        0, 1, 2,
        2, 3, 0
    };
    std::vector<Texture> mapTextures;
    Texture texture = {loadTexture(TEX_EMPTY),"blah","blah"};
    mapTextures.push_back(texture);
    mesh.setVertexData(mapVertices, mapIndices, mapTextures);
    
    shader.use();
    glm::mat4 modelMat = glm::mat4(1.0f);
   // modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(1,0,0));
    modelMat= glm::translate(modelMat, glm::vec3(0,-0.14,0));
    shader.setMat4("modelMat", modelMat);
    
    extern GLuint uboViewProj;
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    glUniformBlockBinding(shader.ID, glGetUniformBlockIndex(shader.ID, "ViewProj"), 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewProj);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    extern GLuint uboLights;
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glUniformBlockBinding(shader.ID, glGetUniformBlockIndex(shader.ID, "Lights"), 1);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Shader& Map::getShader() {
    return shader;
}

Mesh& Map::getMesh() {
    return mesh;
}
