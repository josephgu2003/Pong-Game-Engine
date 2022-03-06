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

AssimpNodeData::AssimpNodeData(std::string& name_,  const aiMatrix4x4& transform,  int parentIndex_) {
    name = name_;
    VertexLoader::ConvertMatrixToGLMFormat(transform, transformation);
    parentIndex = parentIndex_;
}
Animation::Animation() {
     
}
Animation::Animation(aiAnimation* animation, std::map<std::string, BoneData>& map_) {
    ticksPerSec = animation->mTicksPerSecond;
    duration = animation->mDuration;
    name = std::string(animation->mName.C_Str());
    name.erase(0, name.find_first_of("|")+1);
    makeBones(animation, map_);
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
         

        bones.push_back(Bone(boneName,
                        channel)); 
    }
    
}

const std::string& Animation::getName() {
    return name;
}

float Animation::getDuration() {
    return duration;
}


Bone* Animation::findBone(const std::string& name) {
    for (int i = 0; i < bones.size(); i++) {
        if (bones.at(i).getBoneName() == name) {
            return &bones.at(i);
        }
    }
    return nullptr;
}
 


int Animation::getTicksPerSec() {
    return ticksPerSec;
}
 

 
void Animation::updateBoneMatrices(std::vector<glm::mat4>& boneMatrices,std::vector<AssimpNodeData>& boneNodes, std::map<std::string, BoneData>& map_, glm::mat4& globalInverse, float t) {
    
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
           localTransform =  boneMatrices[map_[boneNodes.at((*i).parentIndex).name].id] * localTransform;
        }

        if (map_.find(boneName) != map_.end())
        {
            int index = map_[boneName].id;
   
            boneMatrices[index] = localTransform;
             
        }
        else {
            printf("ERROR: node %s not found in bone map \n", boneName.c_str());
        }
    }
    
    for (auto i = boneNodes.begin(); i != boneNodes.end(); i++) {
        std::string boneName = (*i).name;
        if (map_.find(boneName) != map_.end())
        {
            int index = map_[boneName].id;
            glm::mat4 offset = map_[boneName].offset;
            boneMatrices[index] = globalInverse*boneMatrices[index] * offset;
        }
    }
}
