//
//  Atmosphere.hpp
//  Pong
//
//  Created by Joseph Gu on 1/19/22.
//

#ifndef Atmosphere_hpp
#define Atmosphere_hpp

#include <stdio.h>
#include "GraphicsObject.hpp"

class Atmosphere : public GraphicsObject {
    glm::vec3 skyColor;
public:
    Atmosphere();
    void draw(Renderer *r) override;
    void setSkybox(const std::string& posX, const std::string& negX, const std::string& posY, const std::string& negY, const std::string& posZ, const std::string& negZ);
    glm::vec3 getSkyColor();
    void setSkyColor(const glm::vec3& skyColor_);
};

class Renderer;

class WorldFog {
    Renderer* renderer = nullptr;
public:
    WorldFog();
    WorldFog(Renderer* r);
    void setDistanceFog(float fogDensity, float fogGradient, const glm::vec3& fogColor);
    float fogDensity;
    float fogGradient;
    glm::vec3 fogColor;
};
#endif /* Atmosphere_hpp */
