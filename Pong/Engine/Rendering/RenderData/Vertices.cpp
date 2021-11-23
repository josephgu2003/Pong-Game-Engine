//
//  Vertices.cpp
//  Pong
//
//  Created by Joseph Gu on 11/5/21.
//

#include "Vertices.hpp"


TBNBWVertex::TBNBWVertex() {
    
}
TBNBWVertex::TBNBWVertex(    glm::vec3 Pos_,
          glm::vec3 Normal_,
          glm::vec2 TexCoords_,
          glm::vec3 Tan_,
                   glm::vec3 BiTan_, int* boneIDs_, float* boneWeights_) {
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

TBNVertex::TBNVertex(    glm::vec3 Pos_,
          glm::vec3 Normal_,
          glm::vec2 TexCoords_,
          glm::vec3 Tan_,
          glm::vec3 BiTan_) {
    Pos = Pos_;
    Normal = Normal_;
    TexCoords = TexCoords_;
    Tan = Tan_;
    BiTan = BiTan_;
}

SimpleVertex::SimpleVertex(    glm::vec3 Pos_,
                           glm::vec2 TexCoords_, int arraytexID_) {
    Pos = Pos_;
    TexCoords = TexCoords_;
    arraytexID = arraytexID_;
}
