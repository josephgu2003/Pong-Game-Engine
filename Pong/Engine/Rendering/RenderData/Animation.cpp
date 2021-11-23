//
//  Animation.cpp
//  Pong
//
//  Created by Joseph Gu on 8/8/21.
//

#include "Animation.hpp"
#include <queue>
#include <assimp/postprocess.h>
#include <iostream>

Animation::Animation() {
    
}
Animation::Animation(aiAnimation* animation, const aiScene* scene, std::map<std::string, BoneData>& map_) {
    ticksPerSec = animation->mTicksPerSecond;
    duration = animation->mDuration; 
    name = std::string(animation->mName.C_Str());
    makeBones(animation, map_);
    readAssimpTree(map_, findRootBone(scene, map_));
    globalInverse = glm::inverse(convertMatrixToGLMFormat(findRootBone(scene, map_)->mTransformation));
}
 
void Animation::readAssimpTree( std::map<std::string, BoneData>& map, const aiNode* node) {
        
    std::vector<aiNodeHolder> nodesNextLvl; // precursors
    std::vector<aiNodeHolder> buffer;

    aiNodeHolder holder = {node, -1};
    nodesNextLvl.push_back(holder); 
      
    while (!nodesNextLvl.empty()){ 
        for (int i = 0; i < nodesNextLvl.size(); i++) {
            aiNodeHolder* holder = &nodesNextLvl.at(i);
            
            AssimpNodeData data;
            
            data.name = holder->node->mName.C_Str();
            data.transformation = convertMatrixToGLMFormat(holder->node->mTransformation);
            data.parentIndex = holder->parentIndex;
            boneNodes.push_back(data);
            
            if (map.find(  data.name ) == map.end()) { //want bones not weighted in vertices, but don't want non bones
                BoneData data_; 
                data_.id = map.size();
                map[  data.name ] = data_; 
                printf("Loaded unweighted bone from nodes \n");
            }
              
            int parentIndex = boneNodes.size()-1;
            
            if (holder->node->mChildren == NULL) {
                continue;
                
            }
            
            for (int j = 0; j < holder->node->mNumChildren; j ++) { //buffer next lvl
                aiNodeHolder precursor = {
                    holder->node->mChildren[j], parentIndex
                };
                buffer.push_back(precursor);
            }
        }
        nodesNextLvl = buffer; 
        buffer.clear(); 
    }
    
    int i = 0;
    std::map<std::string, BoneData>::iterator it;
    std::map<std::string, BoneData>::iterator it2;
     
    for (it = map.begin(); it != map.end(); it++)
    {
        for (it2 = map.begin(); it2 != map.end(); it2++) {
            if (it->second.offset == it2->second.offset) {
                i ++;
            }
        }
    }
    std::cout << "Repeat offsets:" << i << "\n";
}
glm::mat4 Animation::convertMatrixToGLMFormat(const aiMatrix4x4& from)
{
    glm::mat4 to;
    //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    
    for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                to[x][y] = from[y][x];
            }
        }
    
    return to;
}

void Animation::makeBones(const aiAnimation* animation, std::map<std::string, BoneData>& map_) {
    int size = animation->mNumChannels;
    
    for (int i = 0; i < size; i++) {
        auto channel = animation->mChannels[i]; 
        std::string boneName(channel->mNodeName.C_Str());
         
        if (map_.find(boneName) == map_.end()) { //if bones in anim channels are not from weighted verices or node heirarchy, add to map
            BoneData data;
            data.id = map_.size();
            map_[boneName] = data;
        } 
         
        int id = map_.at(boneName).id;
        bones.push_back(Bone(boneName,
                        id, channel)); 
    }
    
}

const std::string& Animation::getName() {
    return name;
}

float Animation::getDuration() {
    return duration;
}

std::vector<AssimpNodeData>* Animation::getNodeData() {
    return &boneNodes;
}
Bone* Animation::findBone(const std::string& name) {
    for (int i = 0; i < bones.size(); i++) {
        if (bones.at(i).getBoneName() == name) {
            return &bones.at(i);
        }
    }
    return nullptr;
}
 
  

glm::mat4& Animation::getGlobalInv() {
    return globalInverse;
}

int Animation::getTicksPerSec() {
    return ticksPerSec;
}
 
const aiNode* Animation::findRootBone(const aiScene* scene, std::map<std::string, BoneData>& map_) {
    std::vector<const aiNode*> nodesNextLvl; // precursors
    std::vector<const aiNode*> buffer;
     
    nodesNextLvl.push_back(scene->mRootNode);
    
    while (!nodesNextLvl.empty()){
        for (int i = 0; i < nodesNextLvl.size(); i++) {
            const aiNode* node = nodesNextLvl.at(i);
            
            if (map_.find(node->mName.C_Str()) != map_.end()) {
                return node;
            }
            
            if (node->mChildren == NULL) {
                continue;
                
            }
            
            for (int j = 0; j < node->mNumChildren; j ++) { //buffer next lvl
           const      aiNode* precursor =
                node->mChildren[j];
                buffer.push_back(precursor);
            }
        }
        nodesNextLvl = buffer;
        buffer.clear();
    }
    return nullptr;
}
 
