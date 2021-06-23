//
//  uiPiece.cpp
//  Pong
//
//  Created by Joseph Gu on 6/21/21.
//

#include "uiPiece.hpp"

uiPiece::uiPiece() {

}


void uiPiece::setVertexData(    std::vector<float> vertices_,
                   std::vector<GLuint> indices_,
                   std::vector<GLuint> textures_,
                            glm::vec2 position_) {
    vertices = vertices_;
    indices = indices_;
    textures = textures_;
    position = position_;
}
