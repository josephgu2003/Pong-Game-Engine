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

struct AssimpNodeData
{
    std::string name;
    glm::mat4 transformation;
    int parentIndex;
};

struct aiNodeHolder {
    const aiNode* node;
    int parentIndex;
};

class Animation {     
private:
    glm::mat4 globalInverse;
    std::string name;
    std::vector<Bone> bones; 
    std::vector<AssimpNodeData> boneNodes; 
    int ticksPerSec;
    float duration;
    void readAssimpTree( std::map<std::string, BoneData>& map, const aiNode* node);
    glm::mat4 convertMatrixToGLMFormat(const aiMatrix4x4& from);
    void makeBones(const aiAnimation* animation,  std::map<std::string, BoneData>& map);
    const aiNode* findRootBone(const aiScene* scene, std::map<std::string, BoneData>& map_);
public: 
    Animation();
    Animation(aiAnimation* animation, const aiScene* scene, std::map<std::string, BoneData>& map_);
    const std::string& getName();
    float getDuration();
    std::vector<AssimpNodeData>* getNodeData();
    Bone* findBone(const std::string& name);
    glm::mat4& getGlobalInv();
    int getTicksPerSec();
};


#endif /* Animation_hpp */
