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
#include "VertexData.hpp"
#include "Mesh.hpp"
#include "AssetManager.hpp"

class Map {
    Mesh mesh;
    Shader shader;
    GLuint array2D;
public:
    Frame frame;
    void init();
    Shader& getShader();
    Mesh& getMesh();
    void beginAnimation();
    void tick();
};
#endif /* Map_hpp */
