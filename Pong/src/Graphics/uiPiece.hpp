//
//  uiPiece.hpp
//  Pong
//
//  Created by Joseph Gu on 6/21/21.
//

#ifndef uiPiece_hpp
#define uiPiece_hpp

#include <stdio.h>
#include "Mesh.hpp"
#include <vector>
#include <glm/glm.hpp>

class uiPiece {
protected:
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    std::vector<GLuint> textures;
    glm::vec2 position;
public:
    uiPiece();
    void setVertexData(    std::vector<float> vertices,
                       std::vector<GLuint> indices,
                       std::vector<GLuint> textures,
                       glm::vec2 position);
};

#endif /* uiPiece_hpp */
