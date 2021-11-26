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
#include "AssetManager.hpp"
#include "VertexMesh.hpp"
 
class Shader; 

class Renderer;


class GraphicsObject { // benefits: keep VAO VBO EBO in the entity, not in teh renderer, much simpler, finally a way to unify ur rendering stuff
protected:
    Shader* shader = NULL;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint numIndices;  
    GLuint instanceCount;  
    Material map;
    GLenum drawTarget;
    void makeInstanceBuffer(int dataSize_, int firstAttribLocation, const std::vector<int>& layout, unsigned int instanceCount);
    void updateInstanceBuffer(const std::vector<float>& v);
    void updateVertexBuffer(VertexMesh* vm);
    template <typename T>
    void updateVertexBuffer(VertexMesh* vm) {
        int stride = sizeof(T);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        for (int i = 0; i < vm->vertices.size(); i++) {
            glBufferSubData(GL_ARRAY_BUFFER, i*stride, sizeof(glm::vec3), (void*) &(vm->vertices.at(i)));
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
    }

public:
    GLuint instanceVBO;
    GraphicsObject();
    Shader* getShader();
    void setShader(Shader* shader);
    Material& getTextureMap();
    GLenum getDrawTarget();
    void bind();
    void unbind(); 
    GLuint getNumIndices();
    virtual void draw(Renderer* r) = 0;     //this is where instanced drawing is cancelled-  world object doesn't know about instanced drawing and stuff
    
// - instancing data?
// - If batch with others: share shader, share vao and vbo, vao and vbo become
// same as others, draw call becomes function pointer?
};

// soon need : mesh for fish, mesh is scripted to wave around, graphics need to update per frame
#endif /* Renderable_hpp */

