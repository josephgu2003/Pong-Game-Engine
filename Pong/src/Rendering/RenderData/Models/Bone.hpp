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
    
    int getPosIndex(float t);
    int getRotationIndex(float t);
    int getScalingIndex(float t);
    
    glm::mat4 interpolatePos(float t);
    glm::mat4 interpolateRotation(float t);
    glm::mat4 interpolateScaling(float t);
  //  glm::mat4 invertMatrixToGLMFormat(const aiMatrix4x4& from);
public:
    float getScaleFactor(float lastT, float nextT, float currentT);
    
    Bone(const std::string& name, int ID, const aiNodeAnim* channel);
    
    glm::mat4 getLocalTransform();
    std::string getBoneName() const;
    int getBoneID();
    
    void tick(float timestamp_);
    
};
#endif /* Bone_hpp */
