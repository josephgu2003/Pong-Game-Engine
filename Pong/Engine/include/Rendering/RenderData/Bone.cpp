//
//  Bone.cpp
//  Pong
//
//  Created by Joseph Gu on 8/8/21.
//

#include "Bone.hpp"
#include <iostream>

Bone::Bone(const std::string& name_, const aiNodeAnim* channel) {
    localTransform = glm::mat4(1.0f);
    posTransform = glm::mat4(1.0f);
    rotationTransform = glm::mat4(1.0f);
    scalingTransform = glm::mat4(1.0f);
    prevPosTransform = glm::mat4(1.0f);
    prevRotationTransform = glm::mat4(1.0f);
    prevScalingTransform = glm::mat4(1.0f);
    name = name_;
    numPos = channel->mNumPositionKeys;
    numRotations = channel->mNumRotationKeys;
    numScales = channel->mNumScalingKeys;

    for (int i = 0; i < numPos; i++) {
        aiVector3D aiPos = channel->mPositionKeys[i].mValue;
        float t = (float)channel->mPositionKeys[i].mTime;
        KeyPos newPosKey;
        newPosKey.pos = glm::vec3(aiPos.x, aiPos.y, aiPos.z);
        newPosKey.timestamp = t;
        positions.push_back(newPosKey);
    }
    for (int i = 0; i < numRotations; i++) {
        aiQuaternion aiQuat = channel->mRotationKeys[i].mValue;
        float t = (float)channel->mRotationKeys[i].mTime;
        KeyRotation newKeyRotation;
        newKeyRotation.rotation = glm::quat(aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z);
        newKeyRotation.timestamp = t;
        rotations.push_back(newKeyRotation);
    }
    for (int i = 0; i < numScales; i++) {
        aiVector3D aiScale = channel->mScalingKeys[i].mValue;
        float t = (float)channel->mScalingKeys[i].mTime;
        KeyScale newKeyScale;
        newKeyScale.scale = glm::vec3(aiScale.x, aiScale.y, aiScale.z);
        newKeyScale.timestamp = t;
        scales.push_back(newKeyScale);
    }
}
 

const glm::mat4& Bone::getLocalTransform() { return localTransform; }
std::string Bone::getBoneName() const { return name; }


 
void Bone::tick(float timestamp_) {
    interpolatePos(timestamp_);
    interpolateRotation(timestamp_);
    interpolateScaling(timestamp_);
    localTransform = posTransform * glm::toMat4(rotationTransform) * scalingTransform;
} 
 
float Bone::getScaleFactor(float lastT, float nextT, float currentT)
   {
       float scaleFactor = 0.0f;
       float midWayLength = currentT - lastT;
       float framesDiff = nextT - lastT;
       scaleFactor = midWayLength / framesDiff; 
       return scaleFactor;
   }
 

