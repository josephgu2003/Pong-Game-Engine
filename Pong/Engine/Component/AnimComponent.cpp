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
#include <queue>

AnimComponent::AnimComponent(Actor& actor, const std::string& filePath) : Component(actor) {
    globalInverse = glm::mat4(1.0f);
    updatePriority = 3;
    timeInAnim = 0;
    boneMatrices.reserve(100);
    for (int i = 0; i < 100; i++)
    boneMatrices.push_back(glm::mat4(1.0f));
    try {
    VertexLoader::loadModelAnimations(this, boneNodes, filePath);
    } catch (...) {
        int q = 1;
    }
    stopwatch.resetTime();
    loopCurrent = false;
    starttick = 0;
    endtick = 0;
}

void AnimComponent::tick() {
    if (activeAnim) {
        timeInAnim += (float)activeAnim->getTicksPerSec()*stopwatch.getTime(); // seems that assimp timestamps are in terms of ticks?
        stopwatch.resetTime();
        if ((timeInAnim) > endtick) {
            if (!loopCurrent) {
                playDefault();
            } else {
                timeInAnim = timeInAnim-endtick + starttick;
            }
        }
        activeAnim->updateBoneMatrices(boneMatrices, boneNodes,globalInverse, timeInAnim, dynamic_cast<Positionable*>(actor));
        
        if (actor->getComponent<GraphicsComponent>()) {
            Shader* shader = actor->getComponent<GraphicsComponent>()->getShader();
            shader->use();
            shader->setUniform("animated", true);
            
            for (int i = 1; i < boneMatrices.size(); ++i) {
                shader->setUniform("boneOffsetMatrices[" + std::to_string(i) + "]",
                                   boneMatrices[i]);
            }
        }
    }
    else {
        if (actor->getComponent<GraphicsComponent>()) {
            Shader* shader =  actor->getComponent<GraphicsComponent>()->getShader();
            shader->use();
            shader->setUniform("animated", false);
        }
    }
}

void AnimComponent::addAnimation(aiAnimation* animation, const aiScene* scene) {
    animations.emplace_back(animation, boneNodes);
   // VertexLoader::ConvertMatrixToGLMFormat(findRootBone(scene, BoneDataMap)->mTransformation, globalInverse);
  //  animations.at(animations.size()-1).rootBone = findRootBone(scene, BoneDataMap)->mName.C_Str();
    //globalInverse = glm::inverse(globalInverse);
    globalInverse = glm::mat4(1.0f); 
}   

 
void AnimComponent::playAnim(const std::string& name, bool looped) {
    timeInAnim = 0;
    
    for (int i = 0; i < animations.size(); i++) {
        if (animations.at(i).getName() == name) {
            activeAnim = &animations.at(i);
            loopCurrent = looped;
            starttick = 0; 
            endtick = activeAnim->getDuration();
            //globalInverse = activeAnim->getGlobalInv();
            return;
        }
    }
    printf("Animation %s not found \n", name.c_str());
}  

void AnimComponent::setDefaultAnim(const std::string& name) {
    for (int i = 0; i < animations.size(); i++) {
        if (animations.at(i).getName() == name) {
            defaultAnim = &animations.at(i);
        }
    }
    activeAnim = defaultAnim;
    starttick = 0;
    endtick = activeAnim->getDuration();
}

void AnimComponent::playDefault() {
    activeAnim = defaultAnim;
    timeInAnim = 0;
    starttick = 0;
    endtick = activeAnim->getDuration();
    globalInverse = glm::mat4(1.0f);
}

void AnimComponent::playAnim(const std::string& name, int loopbegin, int loopend) {
    playAnim(name, true);
    starttick = loopbegin;
    endtick = loopend;
}

void AnimComponent::playAnimIfNotPlaying(const std::string& name) {
    if (activeAnim && activeAnim->getName() == name) {
        return;
    } else {
        playAnim(name, true);
    }
}
