//
//  Animation.cpp
//  Pong
//
//  Created by Joseph Gu on 8/8/21.
//

#include "Animation.hpp"
#include <assimp/postprocess.h>
#include <iostream>
#include "VertexLoader.hpp"

BoneNode::BoneNode(std::string& name_,  const aiMatrix4x4& transform,  int parentIndex_) {
    name = name_;
    VertexLoader::ConvertMatrixToGLMFormat(transform, transformation);
    parentIndex = parentIndex_;
}

Animation::Animation() {
     
}

Animation::Animation(aiAnimation* animation, std::vector<BoneNode>& boneNodes) {
    ticksPerSec = animation->mTicksPerSecond;
    duration = animation->mDuration; 
    name = std::string(animation->mName.C_Str());
    name.erase(0, name.find_first_of("|")+1);
    makeBones(animation, boneNodes); 
}
    
void Animation::makeBones(const aiAnimation* animation, std::vector<BoneNode>& boneNodes) {
    
    auto animationUsesBone = [&] (const std::string& name, int& index) {
        for (int i = 0; i < animation->mNumChannels; i++) {
            std::string s = animation->mChannels[i]->mNodeName.C_Str();
            if (s == name) {
                index = i;
                return true; 
            }
        }  
        return false;
    };
     
    for (int i = 0; i < boneNodes.size(); i++) {
        BoneNode& node = boneNodes.at(i);
        int index = 0; 
        if (animationUsesBone(node.name, index)) {
            auto channel = animation->mChannels[index];
            std::string boneName = channel->mNodeName.C_Str();
          
            bones.emplace_back(boneName, channel);
        }
    } 
    
}
 
const std::string& Animation::getName() {
    return name;
}

float Animation::getDuration() {
    return duration;
}


int Animation::getTicksPerSec() {
    return ticksPerSec;
}
 
 
void Animation::updateBoneMatrices(std::vector<glm::mat4>& boneMatrices,std::vector<BoneNode>& boneNodes, glm::mat4& globalInverse, float t, Positionable* p) {
    
    auto findBone = [&] (const std::string name) -> Bone* {
        for (int i = 0; i < bones.size(); i++) {
            Bone& bone = bones.at(i);
            if (bone.getBoneName() == name) {
                return &bone;
            }
        }
        return nullptr;
    };
    
    int count = 0;
    for (auto i = boneNodes.begin(); i != boneNodes.end(); i++) {
        std::string boneName = (*i).name;
        glm::mat4 localTransform = (*i).transformation;
        Bone* bone = findBone(boneName);
        
        if (bone) {
            bone->tick(t);
            localTransform = bone->getLocalTransform();
        }   else {
            localTransform = glm::mat4(1.0f);
        }
        
        if ((*i).parentIndex >= 0) {
           localTransform = boneMatrices[(*i).parentIndex] * localTransform;
        }

        boneMatrices[count] = localTransform;
        
        count++;
        
    }
    
    int count2 = 0;
    for (auto i = boneNodes.begin(); i != boneNodes.end(); i++) {
        std::string boneName = (*i).name;

        glm::mat4 offset = (*i).offset;
        boneMatrices[count2] = globalInverse*boneMatrices[count2] * offset;
        count2++;
    }
    return;
    
    for (int i = 0; i != bones.size(); i++) {
        Bone& bone = bones.at(i);
        std::string boneName = bone.getBoneName();
        
        BoneNode* node = nullptr;
        
        int indexInMatrices = 0;
        for (int i = 0; i != boneNodes.size(); i++) {
            if (boneNodes.at(i).name == boneName) {
                node = &boneNodes.at(i);
                indexInMatrices = i;
                break;
            }
        }
        
        if (!node) {
            throw "Bone not found in master BoneList";
        }
        
        glm::mat4 localTransform = node->transformation;

        bone.tick(t);
        localTransform = bone.getLocalTransform();
        
        if (node->parentIndex >= 0) {
        localTransform = boneMatrices[node->parentIndex] * localTransform;
        boneMatrices[indexInMatrices] = localTransform;
        }

    }
     
    for (int i = 0; i != bones.size(); i++) {
        Bone& bone = bones.at(i);
        std::string boneName = bone.getBoneName(); 
        BoneNode* node = nullptr;
        
        int indexInMatrices = 0;
        
        for (int i = 0; i != boneNodes.size(); i++) {
            if (boneNodes.at(i).name == boneName) {
                node = &boneNodes.at(i);
                indexInMatrices = i;
                break;
            }
        }
        
        if (!node) {
            throw "Bone not found in master BoneList";
        }
        
        glm::mat4 offset = node->offset;
        boneMatrices[indexInMatrices] = globalInverse * boneMatrices[indexInMatrices] * offset;
    }

}
  
