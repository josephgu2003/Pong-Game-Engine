//
//  Map.hpp
//  Pong
//
//  Created by Joseph Gu on 6/24/21.
//

#ifndef Map_hpp
#define Map_hpp

#include <stdio.h>
#include "Shader.hpp"
#include "Mesh.hpp"
class Map {
    Mesh mesh;
    Shader shader;
public:
    void init();
    Shader& getShader();
    Mesh& getMesh();
};
#endif /* Map_hpp */
