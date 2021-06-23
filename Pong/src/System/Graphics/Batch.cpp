//
//  Batch.cpp
//  Pong
//
//  Created by Joseph Gu on 6/20/21.
//

#include "Batch.hpp"
#include <glm/glm.hpp>
#include "Mesh.hpp"

template <typename T>
Batch<T>::Batch(unsigned int maxByteSize_, int vertexType_) {
    maxByteSize = maxByteSize_;
    vertexType = vertexType_;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, maxByteSize, nullptr, GL_STATIC_DRAW);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8*maxByteSize, nullptr, GL_STATIC_DRAW);
    
    if(vertexType == 0) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(sizeof(glm::vec3)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));
        glEnableVertexAttribArray(2);
    }
    
    if (vertexType == 1) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(SimpleVertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  sizeof(SimpleVertex), (void*)(sizeof(glm::vec3)));
        glEnableVertexAttribArray(1);
    }
    
    glBindVertexArray(0);
}
  
template <typename T>
void Batch<T>::addVerticesIndices(const std::vector<T> &vertices, const std::vector<GLuint> &indices) {
    int vDataSize =  sizeof(T)*vertices.size();
    byteStrideVertex += vDataSize;
    if (byteStrideVertex > maxByteSize) return;
    
    int iDataSize = sizeof(GLuint)*indices.size();
    byteStrideIndex += iDataSize;
    if (byteStrideVertex > 8*maxByteSize) return;
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferSubData(GL_ARRAY_BUFFER,byteStrideVertex,vDataSize, vertices.data());
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,byteStrideIndex,iDataSize, indices.data());
    glBindVertexArray(0);
}

template <typename T>
void Batch<T>::bindVAO() {
    glBindVertexArray(VAO);
}

template <typename T>
void Batch<T>::unbindVAO() {
    glBindVertexArray(0);
}
