//
//  Batch.hpp
//  Pong
//
//  Created by Joseph Gu on 6/20/21.
//

#ifndef Batch_hpp
#define Batch_hpp

#include <stdio.h>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>
#define VERTEX_VERTEX 0
#define VERTEX_SIMPLEVERTEX 1

template <typename T>
class Batch {
    unsigned int maxByteSize;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    unsigned int byteStrideVertex, byteStrideIndex = 0;
    unsigned int vertexType;
public:
    Batch(unsigned int maxByteSize_, int vertexType_);
    void addVerticesIndices(const std::vector<T>& vertices, const std::vector<GLuint>& indices);
    void bindVAO();
    void unbindVAO();
};

#endif /* Batch_hpp */
