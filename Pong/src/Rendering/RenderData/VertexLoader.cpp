//
//  VertexLoader.cpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#include "VertexLoader.hpp" 
#include <GL/glew.h> 
#define GLFW_DLL 
#include <GLFW/glfw3.h>  
#include "AssetManager.hpp" 
#include <glm/glm.hpp>         
  
void VertexLoader::loadTextData(const std::string& text, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, TextureMaps& map) {
    glBindVertexArray(vao); 
    
    std::map<char, Character> characters; 

    AssetManager::loadGlyphs("Resources/Glyphs/times.ttf", characters, map);

    std::string::const_iterator c;
    float x = -0.6;
    float y = -0.8;
    float scale = 0.0002;  
        
    std::vector<SimpleVertex> newVertices;
    std::vector<GLuint> newIndices;
        
    int indexOffset = 0;
        
    for (c = text.begin(); c != text.end(); c++)
        {
            if (*c == '\n') {  
                x = -0.6;
                y -= 0.07;
                continue; 
            }
            Character ch = characters[*c];

            float xpos = x + ch.bearing.x * scale;
            float ypos = y - (ch.size.y - ch.bearing.y) * scale;

            float w = ch.size.x * scale;
             
            float h = ch.size.y * scale;
              
            float texX, texY = 0;
              
            texX = ch.texCoords.x;
            texY = ch.texCoords.y;
              
            int id = ch.id;
               
            newVertices.emplace_back(glm::vec3(xpos, ypos + h, 0), glm::vec2(0.0f, 0.0f), id);
            newVertices.emplace_back(glm::vec3(xpos, ypos, 0),  glm::vec2(0.0f, texY), id);
            newVertices.emplace_back(glm::vec3(xpos + w, ypos, 0),  glm::vec2(texX, texY), id);
            newVertices.emplace_back(glm::vec3(xpos + w, ypos + h, 0),  glm::vec2(texX, 0.0f), id);
              
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
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, newVertices.size() * sizeof(SimpleVertex), newVertices.data(), GL_STATIC_DRAW);
    
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, newIndices.size() * sizeof(GLuint), newIndices.data(), GL_STATIC_DRAW);
    
    numIndices = newIndices.size();
    setupVAOAttribs(VERTEX_SIMPLEVERTEX);
    
    glBindVertexArray(0);
}

void VertexLoader::setupVAOAttribs(VertexType vt) {
    switch (vt) {
        case VERTEX_SIMPLEVERTEX:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(SimpleVertex), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  sizeof(SimpleVertex), (void*)(sizeof(glm::vec3)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,  sizeof(SimpleVertex), (void*)(sizeof(glm::vec2) + sizeof(glm::vec3)));
            glEnableVertexAttribArray(2);
            break;
            
        case VERTEX_VERTEX:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(sizeof(glm::vec3)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));
            glEnableVertexAttribArray(2);
            break;
            
        case VERTEX_TBNVERTEX:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)0);
            glEnableVertexAttribArray(0);
            
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(sizeof(glm::vec3)));
            glEnableVertexAttribArray(1);
            
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(2*sizeof(glm::vec3)));
            glEnableVertexAttribArray(2);
            
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(2*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(3);
            
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(3*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(4);
            break;
            
        case VERTEX_TBNBWVERTEX:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)0);
            glEnableVertexAttribArray(0);
            
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(sizeof(glm::vec3)));
            glEnableVertexAttribArray(1);
            
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(2*sizeof(glm::vec3)));
            glEnableVertexAttribArray(2);
            
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(2*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(3);
            
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(3*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(4);
            
            glVertexAttribIPointer(5, MAX_BONE_WEIGHTS, GL_INT, sizeof(TBNBWVertex), (void*)(4*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(5);
            
            glVertexAttribPointer(6, MAX_BONE_WEIGHTS, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(MAX_BONE_WEIGHTS*sizeof(int)+4*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(6);
            
            break;
            
    }
}

void VertexLoader::load2DQuadData(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, glm::vec2 dimensions, glm::vec2 position) {
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    std::vector<SimpleVertex> newVertices = {SimpleVertex(glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0), 0.0), SimpleVertex(glm::vec3(0.0, 1.0, 0.0), glm::vec2(0.0, 1.0), 0.0), SimpleVertex(glm::vec3(1.0, 0.0, 0.0), glm::vec2(1.0, 0.0), 0.0), SimpleVertex(glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0), 0.0)};
    for (int i = 0; i < newVertices.size(); i++) {
        SimpleVertex& sv = newVertices.at(i);
        sv.Pos.x *= dimensions.x;
        sv.Pos.y *= dimensions.y;
        sv.Pos += glm::vec3(position.x, position.y, 0.0f);
    }
    
    glBufferData(GL_ARRAY_BUFFER, newVertices.size() * sizeof(SimpleVertex), newVertices.data(), GL_STATIC_DRAW);
    
std::vector<GLuint> newIndices2 = {
          0, 1, 3, 0, 2, 3
    };
    
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, newIndices2.size() * sizeof(GLuint), newIndices2.data(), GL_STATIC_DRAW);
    
    numIndices = newIndices2.size();
    setupVAOAttribs(VERTEX_SIMPLEVERTEX);
    glBindVertexArray(0);
}
