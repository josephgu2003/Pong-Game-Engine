//
//  AnimComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 8/9/21.
//

#ifndef AnimComponent_hpp
#define AnimComponent_hpp

#include <stdio.h>
#include <vector>
#include "Component.hpp"
#include "Animation.hpp"
#include "Bone.hpp"
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include "Watch.hpp"
class Actor;


typedef std::map<std::string, BoneData> BoneInfoMap;

class AnimComponent : public Component {
protected:
    Watch stopwatch;
    
    float timeInAnim;
    Animation* activeAnim = NULL;
    Animation* defaultAnim = NULL;
    std::vector<Animation> animations;
     
    bool loopCurrent;
    int starttick;
    int endtick;

    glm::mat4 globalInverse;
    std::vector<AssimpNodeData> boneNodes; 
    BoneInfoMap BoneDataMap;
    std::vector<glm::mat4> boneMatrices;

    const aiNode* findRootBone(const aiScene* scene, std::map<std::string, BoneData>& map_);
public:
    AnimComponent(Actor& actor, const std::string& filePath);
    void playDefault(); 
    void readAssimpTree(const aiNode* node);
    virtual void tick() override;
    void setDefaultAnim(const std::string& name);
    void playAnim(const std::string& name, bool looped);
    void playAnim(const std::string& name, int firsttick, int lasttick);
    void addAnimation(aiAnimation* animation, const aiScene* scene);
    void setBoneDataMap(const std::map<std::string, BoneData>& BoneDataMap);
};
#endif /* AnimComponent_hpp */
