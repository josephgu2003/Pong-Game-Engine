//
//  uiText.cpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#include "uiText.hpp"
#include "VertexLoader.hpp"
#include "Shader.hpp"
 
uiText::uiText() : uiPiece::uiPiece() {
    setShader(new Shader("Shaders/Simple2D.vs", "Shaders/TextFShader.fs"));
}
  

void uiText::setText(const std::string &s) {
    VertexLoader::loadTextData(s, VAO, VBO, EBO, numIndices, map);
} 
