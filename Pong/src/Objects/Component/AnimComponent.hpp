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

class AnimComponent : public Component {
protected:
    glm::mat4 globalInverse;
    float timeInAnim;
    Animation* activeAnim = NULL;
    std::vector<Animation> animations;
    std::map<std::string, BoneData> BoneDataMap;
    std::vector<glm::mat4>  boneMatrices; 
    void updateBoneMatrices(float t);
public:
    AnimComponent();
    virtual void tick(Actor& actor, World& world) override;
    void playAnim(const std::string& name);
    void addAnimation(aiAnimation* animation, const aiScene* scene);
    void setBoneDataMap(const std::map<std::string, BoneData>& BoneDataMap);
};
#endif /* AnimComponent_hpp */
