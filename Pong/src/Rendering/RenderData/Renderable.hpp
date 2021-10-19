//
//  Renderable.hpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#ifndef Renderable_hpp
#define Renderable_hpp

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "VertexData.hpp"

class Shader;

class Renderable {
protected:
    Shader* shader = NULL;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint numIndices;
    TextureMaps map;
public:
    Renderable();
    Shader* getShader();
    void setShader(Shader* shader);
    TextureMaps& getTextureMap();
    void bind();
    void unbind();
    GLuint getNumIndices();
// - instancing data?
// - If batch with others: share shader, share vao and vbo, vao and vbo become
// same as others, draw call becomes function pointer?
};

#endif /* Renderable_hpp */

