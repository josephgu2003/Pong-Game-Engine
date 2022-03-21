//
//  Bone.hpp
//  Pong
// 
//  Created by Joseph Gu on 8/8/21.
//

#ifndef Bone_hpp
#define Bone_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "Positionable.hpp"
#include <glm/gtx/matrix_decompose.hpp>

struct BoneData {
    int id;
    glm::mat4 offset;
};

struct KeyPos {
    glm::vec3 pos;
    float timestamp;
};

struct KeyRotation {
    glm::quat rotation;
    float timestamp;
};

struct KeyScale {
    glm::vec3 scale;
    float timestamp;
};


class Bone {
private:
    std::vector<KeyPos> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScale> scales;
    int numPos, numRotations, numScales;
    
    int id;
    std::string name;
    glm::mat4 localTransform;
    
    glm::mat4 posTransform;
    glm::quat rotationTransform;
    glm::mat4 scalingTransform;
    
    glm::mat4 prevPosTransform;
    glm::quat prevRotationTransform;
    glm::mat4 prevScalingTransform;
    
    template<typename keytype>
    inline int getCurrentKeyIndex(const std::vector<keytype>& keys, float t) {
        for (int i = 0; i < keys.size()-1; i++) {
            if ((keys[i].timestamp <= t) && (keys[i+1].timestamp > t)) {
                return i;
            }
        }
        return 0;
    }
    
    inline void interpolatePos(float t) {
        if (1 == numPos)
                    posTransform = glm::translate(glm::mat4(1.0f), positions[0].pos);
        int index = getCurrentKeyIndex<KeyPos>(positions, t);
        float scaleFactor = getScaleFactor(positions[index].timestamp, positions[index+1].timestamp, t);
        glm::vec3 newPos = glm::mix(positions[index].pos, positions[index+1].pos, scaleFactor);
        posTransform = glm::translate(glm::mat4(1.0f), newPos);
    }
    inline void interpolateRotation(float t) {
        if (1 == numRotations) {
            auto rotation = glm::normalize(rotations[0].rotation);
                       rotationTransform = rotation;
        }
        int index = getCurrentKeyIndex<KeyRotation>(rotations, t);
        float scaleFactor = getScaleFactor(rotations[index].timestamp, rotations [index+1].timestamp, t);
        glm::quat newRotation = glm::slerp(rotations[index].rotation,
                                      rotations[index+1].rotation, scaleFactor);
         //    newRotation = glm::normalize(newRotation);
        rotationTransform = newRotation;
    }

    inline void interpolateScaling(float t) {
        if (1 == numScales)
            scalingTransform = glm::scale(glm::mat4(1.0f), scales[0].scale);
        int index = getCurrentKeyIndex<KeyScale>(scales, t);
        float scaleFactor = getScaleFactor(scales[index].timestamp, scales[index+1].timestamp, t);
        glm::vec3 newScale = glm::mix(scales[index].scale, scales[index+1].scale, scaleFactor);

        scalingTransform = glm::scale(glm::mat4(1.0f), newScale);
    }
     

public:
    float getScaleFactor(float lastT, float nextT, float currentT);
    
    Bone(const std::string& name, const aiNodeAnim* channel);
    
    const glm::mat4& getLocalTransform();
    std::string getBoneName() const;
    
    void tick(float timestamp_);
    
    void updatePositionable(Positionable* p, const glm::mat4& parentTransform) {
        glm::vec3 trans; 
        glm::quat rot;
        glm::vec3 scale;
        glm::vec3 skew;
        glm::vec4 persepctive;
        
        glm::decompose(parentTransform, scale, rot, trans, skew, persepctive);
        
        p->updateScaling(glm::scale(scalingTransform, scale) * glm::inverse(prevScalingTransform));
        
        p->updateRotation(rot * rotationTransform * glm::inverse(prevRotationTransform));
        p->updateTranslation(glm::translate(posTransform, trans) * glm::inverse(prevPosTransform));
        
        prevScalingTransform = glm::scale(scalingTransform, scale);
        prevRotationTransform = rot * rotationTransform; 
        prevPosTransform = glm::translate(posTransform, trans);
    }
     
};
#endif /* Bone_hpp */
