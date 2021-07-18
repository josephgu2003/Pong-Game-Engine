//
//  Map.cpp
//  Pong
//
//  Created by Joseph Gu on 6/24/21.
//

#include "Map.hpp"
#include <vector>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

void Map::init() {
    shader.init("Shaders/MapVertexShader.vs", "Shaders/MapFragmentShader.fs");
    std::shared_ptr<TBNVertex> a = std::make_shared<TBNVertex>(glm::vec3(-50,0,-50),glm::vec3(0,1,0),glm::vec2(0,0), glm::vec3(0,0,0), glm::vec3(0,0,0));
    std::shared_ptr<TBNVertex> b = std::make_shared<TBNVertex>(glm::vec3(50,0,-50),glm::vec3(0,1,0),glm::vec2(1,0), glm::vec3(0,0,0), glm::vec3(0,0,0));
    std::shared_ptr<TBNVertex> c = std::make_shared<TBNVertex>(glm::vec3(50,0,50),glm::vec3(0,1,0),glm::vec2(1,1), glm::vec3(0,0,0), glm::vec3(0,0,0));
    std::shared_ptr<TBNVertex> d = std::make_shared<TBNVertex>(glm::vec3(-50,0,50),glm::vec3(0,1,0),glm::vec2(0,1), glm::vec3(0,0,0), glm::vec3(0,0,0));
    std::vector<std::shared_ptr<AnyVertex>> mapVertices = {a,b,c,d
    };
    
    std::vector<GLuint> mapIndices = {
        0, 1, 2,
        2, 3, 0
    };
    std::vector<Texture> mapTextures;
    
    GLuint id;
    loadNullTexture(1600,1600,&id,GL_RGBA);
   Texture texture = {id,"blah","blah"};
    //Texture texture = {loadTexture(TEX_EMPTY),"blah","blah"};
    mapTextures.push_back(texture);
    mesh.setVertexData(mapVertices, mapIndices, mapTextures, VERTEX_TBNVERTEX);
    generateFramebuffer(&frame, &mesh.getTextures().at(0).id, 1600, 1600);
    
    load3DTexture(TEX_SMOKES, array2D);
    
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
    
    extern GLuint uboStopWatch;
    glBindBuffer(GL_UNIFORM_BUFFER, uboStopWatch);
    glUniformBlockBinding(shader.ID, glGetUniformBlockIndex(shader.ID, "StopWatch"), 2);
    glUniformBlockBinding(frame.shader.ID, glGetUniformBlockIndex(frame.shader.ID, "StopWatch"), 2);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboStopWatch);
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Shader& Map::getShader() {
    return shader;
}

Mesh& Map::getMesh() {
    return mesh;
}

void Map::beginAnimation() {
}

void Map::tick() {
    glBindFramebuffer(GL_FRAMEBUFFER, frame.fbo);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(frame.fvao);
    
    frame.shader.use();
    
    glActiveTexture(GL_TEXTURE0); // accomodate more trextures later
    glUniform1i(glGetUniformLocation(frame.shader.ID, "fbotexture"), 0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, array2D);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
