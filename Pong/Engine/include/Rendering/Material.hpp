//
//  Material.hpp
//  Pong
//
//  Created by Joseph Gu on 4/3/22.
//

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <string> 
#include <glm/glm.hpp>

struct Texture {
    GLuint id = -1;
    std::string path;
    glm::vec3 dimensions;
    GLenum textureTarget;
    
    bool operator==(const Texture &other) const {
      return (id == other.id) &&
        (textureTarget == other.textureTarget);
    }
};

struct Material {
    Texture diffuse;
    Texture specular;
    Texture normMap;
    Texture alphaMap;
    float shininess = 0.5;
    
    bool operator==(const Material &other) const {
      return (diffuse == other.diffuse) &&
        (specular == other.specular) &&
        (normMap == other.normMap) &&
        (alphaMap == other.alphaMap) &&
        (shininess == other.shininess);
    }
};


#endif /* Material_hpp */
