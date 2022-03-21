//
//  Animation.hpp
//  Pong
//
//  Created by Joseph Gu on 8/8/21.
// 


#ifndef Animation_hpp
#define Animation_hpp

#include <stdio.h>
#include <string>
#include <glm/glm.hpp>
#include "Model.hpp"
#include "Bone.hpp"
#include <assimp/Importer.hpp>
#include <map>

class Positionable;

struct AssimpNodeData
{
    std::string name;
    glm::mat4 transformation;
    int parentIndex;
    AssimpNodeData(std::string& name, const aiMatrix4x4& transform, int parentIndex);
};

struct aiNodeHolder {
    const aiNode* node;
    int parentIndex;
};

class Animation {
private:
    std::string name;
    std::vector<Bone> bones;
    int ticksPerSec;
    float duration;
    void makeBones(const aiAnimation* animation,  std::map<std::string, BoneData>& map);
public:
    Animation();
    Animation(aiAnimation* animation, std::map<std::string, BoneData>& map_);
    const std::string& getName();
    float getDuration();

    Bone* findBone(const std::string& name);
    int getTicksPerSec();
    void updateBoneMatrices(std::vector<glm::mat4>& boneMatrices, std::vector<AssimpNodeData>& boneNodes, std::map<std::string, BoneData>& map_, glm::mat4& globalInverse, float t,  Positionable* p);
};

 
#endif /* Animation_hpp */
