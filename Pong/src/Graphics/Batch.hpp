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
#include <map>
#include "VertexData.hpp"

struct VertexIndexByteStride {
    unsigned int VertexByteStride;
    unsigned int IndexByteStride;
};

struct InstanceBlockDescription {
    GLuint bufferID;
    unsigned int instanceByteStride;
    unsigned int bytesToEnd;
};
 
class Batch {
    unsigned int maxVerticesSize, maxIndicesSize, maxInstancingByteSize;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    std::vector<std::pair<VertexData*, VertexIndexByteStride>> locations;
    std::vector<std::pair<VertexData*, InstanceBlockDescription>> instanceDataLocations;
    std::vector<int> instanceLayout;
    VertexType vertexType;
    int floatsPerVertex;
    GLenum drawTarget;
    GLenum drawMode;
    bool instancing;
    int bytesPerInstance;
    GLuint makeInstanceBuffer(VertexData* data_, int dataSize);
    void configVAOAttribs();
public:
    Batch();
    
    Batch(unsigned int maxByteSize_, unsigned int maxIndicesSize_, VertexType vertexType_, GLenum drawTarget, GLenum drawMode);
    void init(unsigned int maxByteSize_, unsigned int maxIndicesSize_, VertexType vertexType_, GLenum drawTarget, GLenum drawMode);
    Batch(unsigned int maxVertexByteSize_, unsigned int maxIndicesSize_, std::vector<int> instanceLayout, VertexType vertexType_, GLenum drawTarget, GLenum drawMode);
    void init(unsigned int maxVertexByteSize_, unsigned int maxIndicesSize_, std::vector<int> instanceLayout, VertexType vertexType_, GLenum drawTarget, GLenum drawMode);
    
    void updateVertexData(VertexData* data);
    void deleteVertexData(VertexData* data);
    void updateInstanceData(VertexData* data, std::vector<float>& data_);
    void deleteInstanceData(VertexData* data, std::vector<float>& data_);
    void bindVAO();
    void unbindVAO();
    
    void bindInstanceBuffer(VertexData* data);
    
    std::vector<std::pair<VertexData*, VertexIndexByteStride>>& getVertexDataMap();
    int getIndexByteStride(VertexData* data_);
};

#endif /* Batch_hpp */
