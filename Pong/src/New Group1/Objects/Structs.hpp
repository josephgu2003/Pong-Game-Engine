//
//  Structs.hpp
//  Pong
//
//  Created by Joseph Gu on 6/14/21.
//

#ifndef Structs_hpp
#define Structs_hpp

#include <stdio.h>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Particle.hpp"


struct Texture;
struct Hitbox;
struct Vertex;
struct SimpleVertex;
struct mapMesh;
struct Weather;
struct Quad;
class ParticleEffect;

struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};

struct Hitbox {
    float maxX, minX, maxY, minY, maxZ, minZ;
};

struct Vertex {
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct SimpleVertex {
    glm::vec3 Pos;
    glm::vec2 TexCoords;
};


struct mapMesh {
    float* mapFirstVertex;
    int mapVertexCount;
    GLuint* mapFirstIndex;
    int mapIndexCount;
};

struct Weather {
    bool brightnessExists;
    bool particlesExists;
    float brightness;
    ParticleEffect particleEffect;
};

struct Quad {
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    float size;
    GLuint texture;
    glm::vec3 pos;
    glm::vec3 rotations; //pitch yaw roll
    glm::vec3 force;
};


#endif /* Structs_hpp */
