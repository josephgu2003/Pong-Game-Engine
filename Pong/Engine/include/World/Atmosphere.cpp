//
//  Atmosphere.cpp
//  Pong
//
//  Created by Joseph Gu on 1/19/22.
//

#include "Atmosphere.hpp"
#include "Renderer.hpp"
#include "VertexLoader.hpp"
#include "PPointGraphicsComponent.hpp"
#include "AssetManager.hpp"
#include "PRefreshComponent.hpp"
#include "PGraphicsComponent.hpp" 

Atmosphere::Atmosphere() : GraphicsObject(DRAW_OPAQUE) {
    VertexLoader::loadSimpleCube(VAO, VBO, EBO, numIndices);
    shader = new Shader("Shaders/SkyVertexShader.vs", "Shaders/SkyFragmentShader.fs");
    Renderer::bindShaderUniblock(shader, DistanceFog);
    skyColor = glm::vec3(0.6, 0.6, 1.0);
    shader->use();
    shader->setUniform("skyColor", skyColor);
    makeStars();
} 

void Atmosphere::draw(Renderer *r) {
        r->renderSky(this);
    if (stars) {
        stars->tick(); 
        r->renderStars(stars->getComponent<GraphicsComponent>());
    }
} 

void Atmosphere::makeStars() {
    Material starMat; 
    int numParticles = 1000;
    stars = std::make_unique<ParticleSystem>(numParticles, 100.0);
    Shader* shader = new Shader("Resources/Engine/Stars.vs", "Resources/Engine/Stars.fs");
    std::shared_ptr<GraphicsComponent> pgc =  std::make_shared<PPointGraphicsComponent>(*stars.get(), numParticles, 0.0007, shader, starMat);
    pgc->setColor(2.5, 2.5, 10.0); 
    stars->addComp(pgc);
    stars->addComponent<PRefreshComponent>(*stars.get(), 5.0, 150, 0.2, glm::vec3(2,2,2), glm::vec3(0.0, 0.01, 0.0), glm::vec3(0.0, 0.04, 0.0));
    stars->getComponent<PRefreshComponent>()->refreshAll();
    stars->setPos(glm::vec3(0, 0, 0)); 
    stars->deleteComponent(stars->getComponent<PRefreshComponent>());
} 
 
glm::vec3 Atmosphere::getSkyColor() {
    return skyColor;
} 
void Atmosphere::setSkyColor(const glm::vec3& skyColor_) {
    skyColor = skyColor_;
    shader->use();
    shader->setUniform("skyColor", skyColor); 
}
 
void Atmosphere::setSkybox(const std::string& posX, const std::string& negX, const std::string& posY, const std::string& negY, const std::string& posZ, const std::string& negZ) {
    AssetManager::loadCubeMapTexture(&map.diffuse, posX, negX, posY, negY, posZ, negZ);
}

WorldFog::WorldFog(Renderer* r) {
    renderer = r;
}

void WorldFog::setDistanceFog(float fogDensity_, float fogGradient_, const glm::vec3& fogColor_) {
    fogDensity = fogDensity_;
    fogGradient = fogGradient_;
    fogColor = fogColor_;
    
    if (renderer) {
        renderer->updateDistanceFog(fogDensity, fogGradient, fogColor);
    }
}

WorldFog::WorldFog() {}
