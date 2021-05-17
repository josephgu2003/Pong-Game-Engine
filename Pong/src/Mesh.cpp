//
//  Mesh.cpp
//  Pong
//
//  Created by Joseph Gu on 5/14/21.
//

#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices_,
     std::vector<GLuint> indices_,
           std::vector<Texture> textures_) {
    vertices = vertices_;
    indices = indices_;
    textures = textures_;
}
