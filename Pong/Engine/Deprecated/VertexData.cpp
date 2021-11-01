//
//  VertexData.cpp
//  Pong
//
//  Created by Joseph Gu on 7/14/21.
//

#include "VertexData.hpp"/** 
#include "AssetManager.hpp"
#include <map>

VertexData::VertexData() {
    
}

VertexData::VertexData(VertexTemplate vt) {
    generateTemplate(vt);
}

VertexData::VertexData(const std::string& text) { 
    std::map<char, Character> characters;

    AssetManager::loadGlyphs("Resources/Glyphs/times.ttf", characters, textures);

    std::string::const_iterator c; 
    float x = -0.9; 
    float y = -0.8;  
    float scale = 0.0004; 
    
    std::vector<AnyVertex*> newVertices;
    std::vector<GLuint> newIndices;
    
    int indexOffset = 0;
    
    for (c = text.begin(); c != text.end(); c++)
      {
          Character ch = characters[*c];

          float xpos = x + ch.bearing.x * scale;
          float ypos = y - (ch.size.y - ch.bearing.y) * scale;

          float w = ch.size.x * scale;
         
          float h = ch.size.y * scale;
          
          float texX, texY = 0;
          
          texX = ch.texCoords.x;
          texY = ch.texCoords.y;
          
          int id = ch.id;
          AnyVertex* v1 = new SimpleVertex(glm::vec3(xpos, ypos + h, 0), glm::vec2(0.0f, 0.0f), id);
          AnyVertex* v2 = new SimpleVertex(glm::vec3(xpos, ypos, 0),  glm::vec2(0.0f, texY), id);
          AnyVertex* v3 =  new SimpleVertex(glm::vec3(xpos + w, ypos, 0),  glm::vec2(texX, texY), id);
          AnyVertex* v4 =  new SimpleVertex(glm::vec3(xpos + w, ypos + h, 0),  glm::vec2(texX, 0.0f), id);
           
          newVertices.push_back(v1);
          newVertices.push_back(v2);
          newVertices.push_back(v3); 
          newVertices.push_back(v4);
          
        std::vector<GLuint> newIndices2 = {
              1, 0, 3, 1, 2, 3
          };

          for (int i = 0; i < newIndices2.size(); i++) { 
              GLuint ind = newIndices2.at(i) + indexOffset;
              newIndices.push_back(ind);
          }
          
          x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
          indexOffset += 4;
      } 
    setVertices(newVertices, VERTEX_SIMPLEVERTEX);
    setIndices(newIndices); 
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

            AnyVertex* v1 = new SimpleVertex(glm::vec3(-0.5,-0.5,0), glm::vec2(0,0), 0);
            AnyVertex* v2 = new SimpleVertex(glm::vec3(0.5,-0.5,0), glm::vec2(1,0), 0);
            AnyVertex* v3 = new SimpleVertex(glm::vec3(0.5,0.5,0), glm::vec2(1,1), 0);
            AnyVertex* v4 = new SimpleVertex(glm::vec3(-0.5,0.5,0), glm::vec2(0,1), 0);
        
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

**/
