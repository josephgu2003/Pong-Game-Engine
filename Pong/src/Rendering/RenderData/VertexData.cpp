//
//  VertexData.cpp
//  Pong
//
//  Created by Joseph Gu on 7/14/21.
//

#include "VertexData.hpp"
TBNBWVertex::TBNBWVertex() {
    
}
TBNBWVertex::TBNBWVertex(    glm::vec3 Pos_,
          glm::vec3 Normal_,
          glm::vec2 TexCoords_,
          glm::vec3 Tan_,
                   glm::vec3 BiTan_, int* boneIDs_, float* boneWeights_) {
    Pos = Pos_;
    Normal = Normal_;
    TexCoords = TexCoords_;
    Tan = Tan_;
    BiTan = BiTan_;
    for (int i = 0; i < MAX_BONE_WEIGHTS; i++) {
        boneIDs[i] = boneIDs_[i];
        boneWeights[i] = boneWeights_[i];
    }
} 

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

VertexData::VertexData(std::vector<AnyVertex*>& vertices_,
     std::vector<GLuint>& indices_,
                       TextureMaps& textures_, VertexType vertexType_) {
    setVertexData(vertices_, indices_, textures_, vertexType_);
}

void VertexData::setVertexData(std::vector<AnyVertex*>& vertices_,
             std::vector<GLuint>& indices_,
             TextureMaps& textures_, VertexType vertexType_)  {
   setVertices(vertices_, vertexType_);
    indices = indices_;
    textures = textures_;

}

void VertexData::generateTemplate(VertexTemplate vt) {
   switch (vt) {
        case QUAD_SIMPLE:
            std::vector<GLuint> particleIndices;

            AnyVertex* v1 = new SimpleVertex(glm::vec3(-0.5,-0.5,0), glm::vec2(0,0));
            AnyVertex* v2 = new SimpleVertex(glm::vec3(0.5,-0.5,0), glm::vec2(1,0));
            AnyVertex* v3 = new SimpleVertex(glm::vec3(0.5,0.5,0), glm::vec2(1,1));
            AnyVertex* v4 = new SimpleVertex(glm::vec3(-0.5,0.5,0), glm::vec2(0,1));
        
            std::vector<AnyVertex*> newVertices;
            
            newVertices.push_back(v1);
            newVertices.push_back(v2);
            newVertices.push_back(v3);
            newVertices.push_back(v4);

            setVertices(newVertices, VERTEX_SIMPLEVERTEX);

                std::vector<GLuint> newIndices = {
                    0, 1, 2,
                    2, 3, 0
                };
                particleIndices.insert(particleIndices.end(), newIndices.begin(), newIndices.end());

            setIndices(particleIndices);
            break;
            
    }

}

const std::vector<std::unique_ptr<AnyVertex>>& VertexData::getVertices() {
    return vertices;
} 

void VertexData::setVertices(std::vector<AnyVertex*>& vertices_, VertexType vertexType_) {
    for (int i = 0; i < vertices_.size(); i++) {
       std::unique_ptr<AnyVertex> vertex = std::make_unique<AnyVertex>();
        vertex.reset(vertices_.at(i)); 
        vertices.push_back(std::move(vertex)); 
    }
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

