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
    glm::mat4 globalInverse;
    float timeInAnim;
    Animation* activeAnim = NULL;
    Animation* defaultAnim = NULL;
    std::vector<Animation> animations;
    BoneInfoMap BoneDataMap; 
    std::vector<glm::mat4>  boneMatrices; 
    void updateBoneMatrices(float t);
    void playDefault();
public:
    AnimComponent(Actor& actor, const std::string& filePath);
    virtual void tick() override;
    void setDefaultAnim(const std::string& name);
    void playAnim(const std::string& name);
    void addAnimation(aiAnimation* animation, const aiScene* scene);
    void setBoneDataMap(const std::map<std::string, BoneData>& BoneDataMap);
};
#endif /* AnimComponent_hpp */
