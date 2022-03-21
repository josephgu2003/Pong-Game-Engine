//
//  uiText.cpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#include "uiText.hpp"
#include "VertexLoader.hpp" 
#include "Shader.hpp"
#include <glm/glm.hpp>

uiText::uiText(const std::string& s, float x, float y, float fontsize_, float linespace_): uiPiece::uiPiece(glm::vec2(x,y), glm::vec2(1,1), "Shaders/UI.vs", "Shaders/TextFShader.fs") {
    maxLineLength = 0.0f;
    fontsize = fontsize_;
    linespace = linespace_;
    shader->use(); 
    shader->setUniform("brightness", 0.0f);
    VertexLoader::loadTextData(s, fontsize, linespace, VAO, VBO, EBO, numIndices, getSingularMaterial(), glm::vec3(0));
} 
   
uiText::uiText(const std::string& s, float x, float y, float fontsize_, float linespace_, float maxLineLength_) : uiPiece::uiPiece(glm::vec2(x,y), glm::vec2(1,1), "Shaders/UI.vs", "Shaders/TextFShader.fs") {
    fontsize = fontsize_;
    linespace = linespace_;
    maxLineLength = maxLineLength_;
    shader->use();
    shader->setUniform("brightness", 0.0f);
    VertexLoader::loadTextData(s, fontsize, linespace, maxLineLength, VAO, VBO, EBO, numIndices, getSingularMaterial(), glm::vec3(0));
}

void uiText::setText(const std::string& s) {
    if (maxLineLength > 0.0f) {
    VertexLoader::loadTextData(s, fontsize, linespace, maxLineLength, VAO, VBO, EBO,numIndices, getSingularMaterial(), glm::vec3(0)); 
    } else {
        VertexLoader::loadTextData(s, fontsize, linespace, VAO, VBO, EBO,numIndices, getSingularMaterial(), glm::vec3(0));
    }
}  
  
void uiText::hover(bool b) { 
    if (b) { 
        shader->use();
        shader->setUniform("brightness", 1.0f);
    } else {
        shader->use();
        shader->setUniform("brightness", 0.0f);
    }
}
  
void uiText::setFireCallback(fireCallback f, Game* g_) { // lmao wtf
    callback = f;
    g = g_; 
}

void uiText::fire() {
    if (callback && g) callback(g);
}
 
