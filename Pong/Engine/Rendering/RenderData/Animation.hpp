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

// problems : loading bones in arbitrary order, requires looking up bone name inthe map to get the matrix index

struct BoneNode
{
    // also store index in the bonematrices or make it the same as parentIndex
    
    std::string name;
    glm::mat4 transformation; // bind pose transformation??
    glm::mat4 offset; // converts vertex to bone space
    int parentIndex;
    
    BoneNode(std::string& name, const aiMatrix4x4& transform, int parentIndex);
};

typedef std::vector<BoneNode> BoneList;

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
    void makeBones(const aiAnimation* animation, std::vector<BoneNode>& boneNodes);
public:

    Animation();
    Animation(aiAnimation* animation, std::vector<BoneNode>& boneNodes);
    const std::string& getName(); 
    float getDuration();

    int getTicksPerSec();
    void updateBoneMatrices(std::vector<glm::mat4>& boneMatrices, std::vector<BoneNode>& boneNodes, glm::mat4& globalInverse, float t, Positionable* p);
    
};

 
#endif /* Animation_hpp */
