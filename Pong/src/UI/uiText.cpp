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
uiText::uiText(const std::string& s, float x, float y): uiPiece::uiPiece(glm::vec2(x,y), glm::vec2(1,1)) {
    setShader(new Shader("Shaders/Simple2D.vs", "Shaders/TextFShader.fs"));
    VertexLoader::loadTextData(s, VAO, VBO, EBO, numIndices, map, position);
}
  
void uiText::setText(const std::string& s) {
    VertexLoader::loadTextData(s, VAO, VBO, EBO, numIndices, map, position);
}
  
 
 
