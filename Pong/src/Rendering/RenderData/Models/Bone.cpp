//
//  Bone.cpp
//  Pong
//
//  Created by Joseph Gu on 8/8/21.
//

#include "Bone.hpp"
#include <iostream>

Bone::Bone(const std::string& name_, int ID, const aiNodeAnim* channel) {
    localTransform = glm::mat4(1.0f); 
    id = ID;
    name = name_;
    numPos = channel->mNumPositionKeys;
    numRotations = channel->mNumRotationKeys;
    numScales = channel->mNumScalingKeys;
  //  std::cout << name_<< " "<<numPos << " "<< numScales << " " << numRotations <<  "\n";
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
 

glm::mat4 Bone::getLocalTransform() { return localTransform; }
  std::string Bone::getBoneName() const { return name; }
  int Bone::getBoneID() { return id; }


void Bone::tick(float timestamp_) {
    glm::mat4 a = interpolatePos(timestamp_);
    glm::mat4 b = interpolateRotation(timestamp_);
    glm::mat4 c = interpolateScaling(timestamp_);
    localTransform = a*b*c;
} 
 
 
int Bone::getPosIndex(float t) {
    for (int i = 0; i < numPos -1 ; i ++) {
        if ((positions[i].timestamp <= t) && (positions[i+1].timestamp > t)) {
            return i;
        }
    }
    return 0;
}
int Bone::getRotationIndex(float t) {
    for (int i = 0; i < numRotations -1 ; i ++) {
        if ((rotations[i].timestamp <= t) && (rotations[i+1].timestamp > t)) {
            return i;
        }
    }
    return 0;
}

int Bone::getScalingIndex(float t) {
    for (int i = 0; i < numScales -1 ; i ++) {
        if ((scales[i].timestamp <= t) && (scales[i+1].timestamp > t)) {
            return i;
        }
    }
    return 0;
}


float Bone::getScaleFactor(float lastT, float nextT, float currentT)
   {
       float scaleFactor = 0.0f;
       float midWayLength = currentT - lastT;
       float framesDiff = nextT - lastT;
       scaleFactor = midWayLength / framesDiff; 
       return scaleFactor;
   }
 

glm::mat4 Bone::interpolatePos(float t) { 
    if (1 == numPos)
                return glm::translate(glm::mat4(1.0f), positions[0].pos);
    int index = getPosIndex(t);
    float scaleFactor = getScaleFactor(positions[index].timestamp, positions[index+1].timestamp, t);
    glm::vec3 newPos = glm::mix(positions[index].pos, positions[index+1].pos, scaleFactor);
      return glm::translate(glm::mat4(1.0f), newPos);
}  
glm::mat4 Bone::interpolateRotation(float t) {   
    if (1 == numRotations) { 
        auto rotation = glm::normalize(rotations[0].rotation);
                   return glm::toMat4(rotation);
    }  
    int index = getRotationIndex(t);
    float scaleFactor = getScaleFactor(rotations[index].timestamp, rotations [index+1].timestamp, t);
    glm::quat newRotation = glm::slerp(rotations[index].rotation,
                                  rotations[index+1].rotation, scaleFactor);
     //    newRotation = glm::normalize(newRotation);
        return glm::toMat4(newRotation);
} 

glm::mat4 Bone::interpolateScaling(float t) { 
    if (1 == numScales)
        return glm::scale(glm::mat4(1.0f), scales[0].scale);
    int index = getScalingIndex(t);
    float scaleFactor = getScaleFactor(scales[index].timestamp, scales[index+1].timestamp, t);
    glm::vec3 newScale = glm::mix(scales[index].scale, scales[index+1].scale, scaleFactor);

    return glm::scale(glm::mat4(1.0f), newScale);
}
 
