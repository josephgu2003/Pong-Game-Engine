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
#include "Watch.hpp"

// guide: https://stackoverflow.com/questions/46839586/opengl-object-in-c-raii-class-no-longer-works
class Shader; 

class Renderer;

struct TextureAnimation {
    std::string uniformName;
    int maxFrames;
    int currentFrame;
    float timePerFrame;
    Watch watch;
    TextureAnimation(const std::string& name, int maxFrames, float fps);
};

enum DrawPass {
    DRAW_OPAQUE,
    DRAW_TRANSPARENT
};

// one mesh basically

class GraphicsObject {
    
private:
    GraphicsObject(const GraphicsObject &) = delete;
    GraphicsObject &operator=(const GraphicsObject &) = delete;
    bool movedFrom = false;
protected:
    std::vector<TextureAnimation> textureAnimations;
    
    Material map;
    
    DrawPass drawPass;
    
    Shader* shader = NULL;
    
    void setSingularMaterial(const Material& map);
    Material& getSingularMaterial();
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
    bool deleteDataOnDestruct = true;
    
    GLuint VAO; // vertex "type"
    GLuint instanceVBO;
    GLuint VBO; // vertex data
    GLuint EBO; // vertex data
    GLuint numIndices; // vertex data
    GLuint instanceCount;
    GLenum drawTarget;
    
    static int nonHolders;
    
    GraphicsObject(GraphicsObject&& other) {
        textureAnimations = other.textureAnimations;
        map = other.map;
        drawPass = other.drawPass;
        shader = other.shader;
        VAO = other.VAO;
        instanceVBO = other.instanceVBO;
        VBO = other.VBO;
        EBO = other.EBO;
        numIndices = other.numIndices;
        instanceCount = other.instanceCount;
        drawTarget = other.drawTarget;
        other.movedFrom = true;
        movedFrom = false;
    }

    GraphicsObject &operator=(GraphicsObject&& other) {
        if (this != &other) {
            if (!movedFrom && deleteDataOnDestruct) {
                glDeleteBuffers(1, &VBO);
                glDeleteBuffers(1, &EBO);
                glDeleteBuffers(1, &instanceVBO);
                glDeleteVertexArrays(1, &VAO);
                if(shader) {
                    delete shader;
                }
            }
            textureAnimations = other.textureAnimations;
            map = other.map;
            drawPass = other.drawPass;
            shader = other.shader;
            VAO = other.VAO;
            instanceVBO = other.instanceVBO;
            VBO = other.VBO;
            EBO = other.EBO;
            numIndices = other.numIndices;
            instanceCount = other.instanceCount;
            drawTarget = other.drawTarget;
            other.movedFrom = true;
            if (movedFrom) {
            movedFrom = false;
                nonHolders--;
            }
            nonHolders++;
        }
        return *this;
    }
    
    template <typename T>
    inline void setUniform(const std::string& name, const T& t) {
        shader->use();
        shader->setUniform(name, t);
    }
    GLuint getInstanceCount();
    void animateTextures();
    void setTextureAnimation(std::string frameIndexUniform, int numFrames, float fps);
    DrawPass getDrawPass(); 
    virtual ~GraphicsObject();
    GraphicsObject(DrawPass dp);
    Shader* getShader();
    void setShader(Shader* shader);
    Material& getTextureMap();
    GLenum getDrawTarget();
    void bind();
    void unbind();
    GLuint getNumIndices();
    bool isInstanced();
    
    virtual void draw(Renderer* r) = 0;     //this is where instanced drawing is cancelled-  world object doesn't know about instanced drawing and stuff
    
    // - instancing data?
    // - If batch with others: share shader, share vao and vbo, vao and vbo become
    // same as others, draw call becomes function pointer?
    void makeInstanceBuffer(int dataSize_, int firstAttribLocation, const std::vector<int>& layout, unsigned int instanceCount);
    void updateInstanceBuffer(const std::vector<float>& v);
};

// multiple meshes : different materials
// batch those of same material together

// 
#endif /* Renderable_hpp */

