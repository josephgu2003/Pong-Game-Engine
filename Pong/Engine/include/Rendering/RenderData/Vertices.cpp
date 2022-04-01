//
//  Vertices.cpp
//  Pong
//
//  Created by Joseph Gu on 11/5/21.
//

#include "Vertices.hpp"


TBNBWVertex::TBNBWVertex() {
    
}
TBNBWVertex::TBNBWVertex(    const glm::vec3&  Pos_,
          const glm::vec3&  Normal_,
          const glm::vec2&  TexCoords_,
          const glm::vec3&  Tan_,
                   const glm::vec3&  BiTan_, int* boneIDs_, float* boneWeights_) {
    Pos = Pos_;
    Normal = Normal_;
    TexCoords = TexCoords_;
    Tan = Tan_;
    BiTan = BiTan_;
    for (int i = 0; i < MAX_BONE_WEIGHTS; i++) {
        boneIDs[i] = boneIDs_[i];
        boneWeights[i] = boneWeights_[i];
    }
}

TBNVertex::TBNVertex() {
    
}

TBNVertex::TBNVertex(    const glm::vec3&  Pos_,
          const glm::vec3&  Normal_,
          const glm::vec2&  TexCoords_,
          const glm::vec3&  Tan_,
          const glm::vec3&  BiTan_) {
    Pos = Pos_;
    Normal = Normal_;
    TexCoords = TexCoords_;
    Tan = Tan_;
    BiTan = BiTan_;
}

TBNMVertex::TBNMVertex(const glm::vec3&  Pos_,
                       const glm::vec3&  Normal_,
                       const glm::vec2&  TexCoords_,
                       const glm::vec3&  Tan_,
                       const glm::vec3&  BiTan_, unsigned int matIndex_) {
    Pos = Pos_;
    Normal = Normal_;
    TexCoords = TexCoords_;
    Tan = Tan_;
    BiTan = BiTan_;
    matIndex = matIndex_;
}

TBNMVertex::TBNMVertex() {
    
}

SimpleVertex::SimpleVertex(    const glm::vec3&  Pos_,
                           const glm::vec2&  TexCoords_, int arraytexID_) {
    Pos = Pos_;
    TexCoords = TexCoords_;
    arraytexID = arraytexID_;
}
