//
//  Map.cpp
//  Pong
//
//  Created by Joseph Gu on 6/24/21.
//

#include "MapObject.hpp"
#include <vector>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "AssetManager.hpp"

void MapObject::init() {
    pos = glm::vec3(0,0,0);

  /**  glm::mat4 modelMat = glm::mat4(1.0f);
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
    glUniformBlockBinding(frameShader.ID, glGetUniformBlockIndex(frameShader.ID, "StopWatch"), 2);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboStopWatch);
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);**/
}
void MapObject::init(glm::vec3 pos_) {
    pos = pos_;

}
void MapObject::setPos(glm::vec3 pos_) {
    pos = pos_;
}
glm::vec3 MapObject::getPos() {
    return pos;
}


void MapObject::beginAnimation() {
}

void MapObject::setGraphics(GraphicsComponent* graphics_) {
    graphics = graphics_;
}
GraphicsComponent& MapObject::getGraphics() {
    return *graphics;
}

void MapObject::tick() {
  /**  glBindFramebuffer(GL_FRAMEBUFFER, frame.fbo);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(frame.fvao);
    
    frameShader.use();
    
    glActiveTexture(GL_TEXTURE0); // accomodate more trextures later
    glUniform1i(glGetUniformLocation(frameShader.ID, "fbotexture"), 0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, array2D);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);**/
}
