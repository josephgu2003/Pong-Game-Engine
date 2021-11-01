//
//  AnimComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 8/9/21.
//

#include "AnimComponent.hpp"
#include <assimp/scene.h>
#include "Actor.hpp"
#include "VertexLoader.hpp"

AnimComponent::AnimComponent(Actor& actor, const std::string& filePath) : ActorComp(actor) {
    globalInverse = glm::mat4(1.0f);
    Component::type = ANIM;
    timeInAnim = 0; 
    boneMatrices.reserve(100);
          for (int i = 0; i < 100; i++)
              boneMatrices.push_back(glm::mat4(1.0f));
    VertexLoader::loadModelAnimations(this, filePath);
}

void AnimComponent::tick() {
    if (activeAnim) {
        timeInAnim += (float)activeAnim->getTicksPerSec()*glfwGetTime();
        timeInAnim = (float)fmod((double)timeInAnim, (double)activeAnim->getDuration());
        updateBoneMatrices(timeInAnim); 
   
        if (actor->getComponent<GraphicsComponent>()) {
            Shader* shader =  actor->getComponent<GraphicsComponent>()->getShader();
            shader->use();
            shader->setBool("animated", true );
            
            for (int i = 0; i < boneMatrices.size(); ++i) {
                        shader->setMat4("boneOffsetMatrices[" + std::to_string(i) + "]",
                                          boneMatrices[i]);
            }
        }
    }
    else {
               if (actor->getComponent<GraphicsComponent>()) {
        Shader* shader =  actor->getComponent<GraphicsComponent>()->getShader();
                   shader->use();
        shader->setBool("animated", false);
               }
    }
}

void AnimComponent::updateBoneMatrices(float t) {
    std::vector<AssimpNodeData>* nodeData = activeAnim->getNodeData();
    
    for (int i = 0; i < nodeData->size() ; i++) {
        std::string boneName = nodeData->at(i).name;
        glm::mat4 localTransform = nodeData->at(i).transformation;
        Bone* bone = activeAnim->findBone(boneName);
        
        if (bone ) {
            bone->tick(t);
            localTransform = bone->getLocalTransform();
        }
        else {
            localTransform = glm::mat4(1.0f);
        }  
        if (nodeData->at(i).parentIndex >= 0) {
           localTransform =  boneMatrices[BoneDataMap[nodeData->at(nodeData->at(i).parentIndex).name].id] * localTransform;
            if ( nodeData->at(i).parentIndex >= i) {
                printf("Critical animation error: node parent is not before in array");
            }
        }

        if (BoneDataMap.find(boneName) != BoneDataMap.end())
        {
            int index = BoneDataMap[boneName].id;
   
            boneMatrices[index] = localTransform;

        }
        else {
            printf("ERROR: AnimComponent's transform node not found in final bone matrices \n");
        }
    } 
    for (int i = 0; i < nodeData->size() ; i++) {
        std::string boneName = nodeData->at(i).name;
        if (BoneDataMap.find(boneName) != BoneDataMap.end())
        {
            int index = BoneDataMap[boneName].id;
            glm::mat4 offset = BoneDataMap[boneName].offset;
 
               boneMatrices[index] = globalInverse*boneMatrices[index] * offset;
        }
    }
}

void AnimComponent::addAnimation(aiAnimation* animation, const aiScene* scene) {
    animations.emplace_back(animation, scene, BoneDataMap);
}   

void AnimComponent::setBoneDataMap(const std::map<std::string, BoneData>& BoneDataMap_) {
    BoneDataMap = BoneDataMap_;

} 
 
void AnimComponent::playAnim(const std::string& name) {
    timeInAnim = 0; 
    for (int i = 0; i < animations.size(); i++) {
        if (animations.at(i).getName() == name) {
            activeAnim = &animations.at(i);
            globalInverse = activeAnim->getGlobalInv();
            return;
        } 
    }
} 

void AnimComponent::setDefaultAnim(const std::string& name) {
    for (int i = 0; i < animations.size(); i++) {
        if (animations.at(i).getName() == name) {
            defaultAnim = &animations.at(i);
        }
    }
    activeAnim = defaultAnim;
}
