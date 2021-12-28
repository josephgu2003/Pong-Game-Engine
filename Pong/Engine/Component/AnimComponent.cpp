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
    Component::type = ANIM;
    timeInAnim = 0; 
    boneMatrices.reserve(100);
          for (int i = 0; i < 100; i++)
              boneMatrices.push_back(glm::mat4(1.0f));
    VertexLoader::loadModelAnimations(this, filePath);
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
        activeAnim->updateBoneMatrices(boneMatrices, boneNodes, BoneDataMap,globalInverse,timeInAnim);
       
        if (actor->getComponent<GraphicsComponent>()) {
            Shader* shader =  actor->getComponent<GraphicsComponent>()->getShader();
            shader->use();
            shader->setUniform("animated", true );
            
            for (int i = 0; i < boneMatrices.size(); ++i) {
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
    animations.emplace_back(animation, BoneDataMap); 
    VertexLoader::ConvertMatrixToGLMFormat(findRootBone(scene, BoneDataMap)->mTransformation, globalInverse);
    globalInverse = glm::inverse(globalInverse);
    globalInverse = glm::mat4(1.0f);
}   
 
void AnimComponent::setBoneDataMap(const std::map<std::string, BoneData>& BoneDataMap_) {
    BoneDataMap = BoneDataMap_;

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

const aiNode* AnimComponent::findRootBone(const aiScene* scene, std::map<std::string, BoneData>& map_) {
    std::vector<const aiNode*> nodesNextLvl; // precursors
    std::vector<const aiNode*> buffer;
     
    nodesNextLvl.push_back(scene->mRootNode);
    
    while (!nodesNextLvl.empty()){
        for (int i = 0; i < nodesNextLvl.size(); i++) {
            const aiNode* node = nodesNextLvl.at(i);
            
            if (map_.find(node->mName.C_Str()) != map_.end()) {
                return node;
            }
            
            if (node->mChildren == NULL) {
                continue;
                
            }
            
            for (int j = 0; j < node->mNumChildren; j ++) { //buffer next lvl
           const      aiNode* precursor =
                node->mChildren[j];
                buffer.push_back(precursor);
            }
        }
        nodesNextLvl = buffer;
        buffer.clear();
    }
    return nullptr;
}


void AnimComponent::readAssimpTree(const aiNode* node) {
 std::queue<const aiNode*> nodes;
 std::queue<int> parentIndices;
 auto checkBoneNameInMap = [] ( std::map<std::string, BoneData>& map, std::string& boneName) {
     if (map.find(boneName) == map.end()) { //want bones not weighted in vertices, but don't want non bones
         BoneData data_;
         data_.id = map.size();
         map[boneName] = data_;
         printf("Loaded bone %s not relevant to vertex weights from nodes \n", boneName.c_str());
     }
 };
  
 nodes.push(node);
 parentIndices.push(-1);
 
 while (!nodes.empty()) {
     const aiNode* aNode = nodes.front();
     nodes.pop();
     int parentIndex = parentIndices.front();
     parentIndices.pop();
 
     std::string bonename = aNode->mName.C_Str();
     
     if (bonename == "knight_RetopoFlow.001_RetopoFlow.003") {
         
     }

     checkBoneNameInMap(BoneDataMap, bonename);
     
     boneNodes.emplace_back(bonename, aNode->mTransformation, parentIndex);
      
     int thisIndex = boneNodes.size()-1;
     for (int i = 0; i < aNode->mNumChildren; i++) {
         nodes.push(aNode->mChildren[i]);
         parentIndices.push(thisIndex);
     }
 }

 int i = 0; // debugging purposes
 std::map<std::string, BoneData>::iterator it;
 std::map<std::string, BoneData>::iterator it2;
  
 for (it = BoneDataMap.begin(); it != BoneDataMap.end(); it++)
 {
     for (it2 = BoneDataMap.begin(); it2 != BoneDataMap.end(); it2++) {
         if (it->second.offset == it2->second.offset) {
             i ++;
         }
     } 
 }
    printf("Repeat offsets: %i\n", i); 
} 

void AnimComponent::playAnim(const std::string& name, int loopbegin, int loopend) {
    playAnim(name, true);
    starttick = loopbegin;
    endtick = loopend;
}
 
