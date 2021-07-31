//
//  VertexData.cpp
//  Pong
//
//  Created by Joseph Gu on 7/14/21.
//

#include "VertexData.hpp"
TBNVertex::TBNVertex() {
    
}
TBNVertex::TBNVertex(    glm::vec3 Pos_,
          glm::vec3 Normal_,
          glm::vec2 TexCoords_,
          glm::vec3 Tan_,
          glm::vec3 BiTan_) {
    Pos = Pos_;
    Normal = Normal_;
    TexCoords = TexCoords_;
    Tan = Tan_;
    BiTan = BiTan_;
}

SimpleVertex::SimpleVertex(    glm::vec3 Pos_,
                           glm::vec2 TexCoords_) {
    Pos = Pos_;
    TexCoords = TexCoords_;
}

VertexData::VertexData() {
    
}

VertexData::VertexData(std::vector<std::shared_ptr<AnyVertex>>& vertices_,
     std::vector<GLuint>& indices_,
           TextureMaps& textures_, VertexType vertexType_) {
    vertices = vertices_;
    indices = indices_;
    textures = textures_;
    vertexType = vertexType_;
}

void VertexData::setVertexData(std::vector<std::shared_ptr<AnyVertex>>& vertices_,
             std::vector<GLuint>& indices_,
             TextureMaps& textures_, VertexType vertexType_)  {
    vertices = vertices_;
    indices = indices_;
    textures = textures_;
    vertexType = vertexType_;
}

std::vector<std::shared_ptr<AnyVertex>>& VertexData::getVertices() {
    return vertices;
}


void VertexData::setVertices(std::vector<std::shared_ptr<AnyVertex>>& vertices_, VertexType vertexType_) {
    vertices = vertices_;
    vertexType = vertexType_;
}

std::vector<GLuint>& VertexData::getIndices() {
    return indices;
}

void VertexData::setIndices(std::vector<GLuint>& indices_) {
    indices = indices_;
}

TextureMaps& VertexData::getTextures() {
    return textures;
}

void VertexData::setTextures(TextureMaps& textures_) { 
    textures = textures_;
}
  
 
  
 
