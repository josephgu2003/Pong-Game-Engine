//
//  Batch.cpp
//  Pong
//
//  Created by Joseph Gu on 6/20/21.
//

#include "Batch.hpp"
#include <glm/glm.hpp>
#include "Mesh.hpp" 

Batch::Batch() {
    
}

Batch::Batch(unsigned int maxByteSize_, unsigned int maxIndicesSize_, VertexType vertexType_, GLenum drawTarget_, GLenum drawMode_) {
    init(maxByteSize_, maxIndicesSize_, vertexType_, drawTarget_, drawMode_);
}
  
void Batch::init(unsigned int maxByteSize_, unsigned int maxIndicesSize_, VertexType vertexType_, GLenum drawTarget_, GLenum drawMode_) {
    vertexType = vertexType_;
    if (vertexType == VERTEX_VERTEX) floatsPerVertex = 8;
    if (vertexType == VERTEX_SIMPLEVERTEX) floatsPerVertex = 5;
    if (vertexType == VERTEX_TBNVERTEX) floatsPerVertex = 14;
    instancing = false;
    maxVerticesSize = maxByteSize_;
    maxIndicesSize = maxIndicesSize_;
    drawTarget = drawTarget_;
    drawMode = drawMode_;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, maxVerticesSize, nullptr, drawMode);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndicesSize, nullptr, drawMode);
    
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
    
    if (vertexType == 2) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(sizeof(glm::vec3)));
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(2*sizeof(glm::vec3)));
        glEnableVertexAttribArray(2);
        
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(2*sizeof(glm::vec3)+sizeof(glm::vec2)));
        glEnableVertexAttribArray(3);
        
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(3*sizeof(glm::vec3)+sizeof(glm::vec2)));
        glEnableVertexAttribArray(4);
    }
    
    glBindVertexArray(0);
}


Batch::Batch(unsigned int maxVertexByteSize_, unsigned int maxIndicesSize_, std::vector<int> instanceLayout, VertexType vertexType_, GLenum drawTarget, GLenum drawMode) {
    init(maxVertexByteSize_, maxIndicesSize_,instanceLayout,vertexType_, drawTarget, drawMode);
}
void Batch::init(unsigned int maxVertexByteSize_, unsigned int maxIndicesSize_,std::vector<int> instanceLayout_, VertexType vertexType_, GLenum drawTarget_, GLenum drawMode_) {
    vertexType = vertexType_;
    if (vertexType == VERTEX_VERTEX) floatsPerVertex = 8;
    if (vertexType == VERTEX_SIMPLEVERTEX) floatsPerVertex = 5;
    if (vertexType == VERTEX_TBNVERTEX) floatsPerVertex = 14;
    instancing = true;
    maxVerticesSize = maxVertexByteSize_;
    maxIndicesSize = maxIndicesSize_;
    drawTarget = drawTarget_;
    drawMode = drawMode_;
    instanceLayout = instanceLayout_;
    
    for (int i = 0; i < instanceLayout.size(); i++) {
        if (instanceLayout.at(i) > 4) { // if needs extra slots
            
            if ((instanceLayout.at(i) % 4) == 0) { // if it is right multiple
                std::vector<int> newLayout;
                for (int c = 0; c < instanceLayout.at(i) / 4; c++) {
                    newLayout.push_back(4);
                }
                instanceLayout.insert(instanceLayout.begin()+i+1, newLayout.begin(), newLayout.end());
                instanceLayout.erase(instanceLayout.begin()+i);
            } else {
                printf("Error: instanceLayout has non multiple of 4 > 4 \n");
                return;
            }
        }
    }
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, maxVerticesSize, nullptr, drawMode);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndicesSize, nullptr, drawMode);
    
    int floatsPerInstance_ = 0;
    for (int i = 0; i < instanceLayout.size(); i ++) {
        floatsPerInstance_ += instanceLayout.at(i);
    }
    bytesPerInstance = floatsPerInstance_ * sizeof(float);
    int byteCounter = 0;

    if(vertexType == 0) {
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(sizeof(glm::vec3)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    if (vertexType == 1) {

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(SimpleVertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  sizeof(SimpleVertex), (void*)(sizeof(glm::vec3)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    if (vertexType == 2) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(sizeof(glm::vec3)));
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(2*sizeof(glm::vec3)));
        glEnableVertexAttribArray(2);
        
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(2*sizeof(glm::vec3)+sizeof(glm::vec2)));
        glEnableVertexAttribArray(3);
        
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(3*sizeof(glm::vec3)+sizeof(glm::vec2)));
        glEnableVertexAttribArray(4);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    glBindVertexArray(0);
}

GLuint Batch::makeInstanceBuffer(VertexData* data_, int dataSize_) {
    GLuint instanceVBO;
    bindVAO();
    glGenBuffers(1,  &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, dataSize_, nullptr, drawMode);
    configVAOAttribs();
    unbindVAO();
    return instanceVBO;
}

void Batch::configVAOAttribs() {
    int byteCounter = 0;
    if(vertexType == 0) {
        for (int i = 0; i < instanceLayout.size(); i++) {
            glVertexAttribPointer(i+3, instanceLayout.at(i), GL_FLOAT, GL_FALSE, bytesPerInstance, (void*)(byteCounter));
        glEnableVertexAttribArray(i+3);
        glVertexAttribDivisor(i+3, 1);
            byteCounter += instanceLayout.at(i)*sizeof(float);
        }    
    }
    
    if (vertexType == 1) {
        for (int i = 0; i < instanceLayout.size(); i++) {
        }
        for (int i = 0; i < instanceLayout.size(); i++) {
            glVertexAttribPointer(i+2, instanceLayout.at(i), GL_FLOAT, GL_FALSE, bytesPerInstance, (void*)(byteCounter));
        glEnableVertexAttribArray(i+2);
        glVertexAttribDivisor(i+2, 1);
            byteCounter += instanceLayout.at(i)*sizeof(float);
        }
        
    }
    
    if (vertexType == 2) {
        for (int i = 0; i < instanceLayout.size(); i++) {
            glVertexAttribPointer(i+5, instanceLayout.at(i), GL_FLOAT, GL_FALSE, bytesPerInstance, (void*)(byteCounter));
        glEnableVertexAttribArray(i+5);
        glVertexAttribDivisor(i+5, 1);
            byteCounter += instanceLayout.at(i)*sizeof(float);
        }
    }
}

void Batch::updateVertexData(VertexData *data) {
    int bytePerVertex;
    
    //lol vertices are below
    std::vector<GLuint>& indices = data->getIndices();
    
    bytePerVertex = floatsPerVertex*sizeof(float);

    
    unsigned int vertexByteStride, indexByteStride = 0;
    
    bool newData = true;
    for (int i = 0; i < locations.size(); i++) {
        if (data == locations.at(i).first) {
            vertexByteStride = locations.at(i).second.VertexByteStride;
            indexByteStride = locations.at(i).second.IndexByteStride;
            newData = false;
        }
    }
    if (newData) {
        if (locations.size()>0){
            vertexByteStride = (--locations.end())->second.VertexByteStride+(--locations.end())->first->getVertices().size()*bytePerVertex;
            indexByteStride = (--locations.end())->second.IndexByteStride+(--locations.end())->first->getIndices().size()*sizeof(GLuint);
        }
        else {
            vertexByteStride = 0;
            indexByteStride = 0;
        }
    }

    int vDataSize = bytePerVertex * data->getVertices().size();


    if ((vDataSize + vertexByteStride) > maxVerticesSize) {
        printf("Batch vertices overloaded \n");
        return;
    }
    
    int iDataSize = sizeof(GLuint)*indices.size();
    
    if ((indexByteStride + iDataSize) > maxIndicesSize)        { printf("Batch indices overloaded \n");return;}
    if (newData) {
    VertexIndexByteStride strides{vertexByteStride, indexByteStride};
        locations.push_back(std::pair<VertexData*, VertexIndexByteStride>(data, strides));
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    std::vector<float> array;
    for (int i = 0; i < data->getVertices().size(); i++) {
        float* vertex = (float*)(data->getVertices().at(i).get());
        for (int j = 0; j < floatsPerVertex; j++) {
            array.push_back(*(vertex+j));
        }
    }
    glBufferSubData(GL_ARRAY_BUFFER,    vertexByteStride,vDataSize, array.data());
    
}

void Batch::deleteVertexData(VertexData* data) {
    int bytePerVertex;
    int position = 0;
    //lol vertices are below
    std::vector<GLuint>& indices = data->getIndices();
    
    bytePerVertex = floatsPerVertex*sizeof(float);

    int vertexByteStride, indexByteStride = -1;

    for (int i = 0; i < locations.size(); i++) {
        if (data == locations.at(i).first) {
            vertexByteStride = locations.at(i).second.VertexByteStride;
            indexByteStride = locations.at(i).second.IndexByteStride;
            position = i;
        }
    }
    if(vertexByteStride == -1) {
        printf("Oops tried to delete vertex data that wasn't there \n");
        return;
    }
    if(indexByteStride == -1) {
        printf("Oops tried to delete vertex data that wasn't there \n");
        return;
    }
    
    
    int vDataSize = bytePerVertex * data->getVertices().size();
    
    int iDataSize = sizeof(GLuint)*indices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferSubData(GL_ARRAY_BUFFER,    vertexByteStride,vDataSize, NULL);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexByteStride,iDataSize, NULL);
     

    locations.erase(locations.begin()+position);
}
      
void Batch::updateInstanceData(VertexData* data, std::vector<float>& data_) {
    unsigned int instanceByteStride = 0;
    
    bool newData = true;
    for (int i = 0; i < instanceDataLocations.size(); i++) {
        if (data == instanceDataLocations.at(i).first) {
            instanceByteStride = instanceDataLocations.at(i).second.instanceByteStride;
            newData = false;
        }
    }     
    if (newData) {
        if (instanceDataLocations.size()>0){
            instanceByteStride = (--instanceDataLocations.end())->second.instanceByteStride+(--instanceDataLocations.end())->second.bytesToEnd;
        }
        else {
            instanceByteStride = 0;
        }
    }
    
    unsigned int instanceDataSize = sizeof(float)*data_.size();
    
    if (newData) {
        GLuint id = makeInstanceBuffer(data, instanceDataSize);
    InstanceBlockDescription descript {id, instanceByteStride, instanceDataSize};
    instanceDataLocations.push_back(std::pair<VertexData*, InstanceBlockDescription>(data, descript));
    }
    
    for (int i = 0; i < instanceDataLocations.size(); i++) {
        if (data == instanceDataLocations.at(i).first) {
            glBindBuffer(GL_ARRAY_BUFFER, instanceDataLocations.at(i).second.bufferID);
        }
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, instanceDataSize, data_.data());

}
void Batch::deleteInstanceData(VertexData* data, std::vector<float>& data_) {
    int position = 0;
    unsigned int instanceByteStride = -1;
    
    
    for (int i = 0; i < instanceDataLocations.size(); i++) {
        if (data == instanceDataLocations.at(i).first) {
            instanceByteStride = instanceDataLocations.at(i).second.instanceByteStride;
            position = i;
        }
    }
    if(instanceByteStride == -1) {
        printf("Oops tried to delete vertex data that wasn't there \n");
        return;
    }
    
    int instanceDataSize = sizeof(float)*data_.size();
    

    glBindVertexArray(VAO);
    bindInstanceBuffer(data);

    glBufferSubData(GL_ARRAY_BUFFER, instanceByteStride, instanceDataSize, NULL);
    
    
    glBindVertexArray(0);
    locations.erase(locations.begin()+position);
}

void Batch::bindVAO() {
    glBindVertexArray(VAO);
}

void Batch::unbindVAO() {
    glBindVertexArray(0);
}

std::vector<std::pair<VertexData*, VertexIndexByteStride>>& Batch::getVertexDataMap() {
    return locations;
}

int Batch::getIndexByteStride(VertexData* data_) {
    int indexByteStride = -1;
    for (int i = 0; i < locations.size(); i++) {
        if (data_ == locations.at(i).first) {
            indexByteStride = locations.at(i).second.IndexByteStride;
        }
    }
    if(indexByteStride == -1) {
        printf("Oops tried to delete vertex data that wasn't there \n");
        return -1;
    }
    return indexByteStride;
}

void Batch::bindInstanceBuffer(VertexData* data) {
    for (int i = 0; i < instanceDataLocations.size(); i++) {
        if (data == instanceDataLocations.at(i).first) {
            glBindBuffer(GL_ARRAY_BUFFER, instanceDataLocations.at(i).second.bufferID);
            configVAOAttribs();
            return;
        }
    }
}
