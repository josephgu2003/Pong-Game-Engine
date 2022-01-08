//
//  uiPiece.cpp
//  Pong
//
//  Created by Joseph Gu on 6/21/21.
//

#include "uiPiece.hpp"
#include "Renderer.hpp"

uiPiece::uiPiece(glm::vec2 position_, glm::vec2 dimensions_, std::string vs, std::string fs) : GraphicsObject::GraphicsObject(DRAW_TRANSPARENT) {
    position = position_; 
    dimensions = dimensions_;
    
    setShader(new Shader(vs.c_str(), fs.c_str()));
    shader->use();
    shader->setUniform("position", glm::vec3(position_.x, position_.y, 0));
} 


void uiPiece::draw(Renderer* r) { 
    r->renderGeneric(this); 
}
 
void uiPiece::insertChild(const std::shared_ptr<uiPiece>& uip) {
    std::shared_ptr<uiPiece> ui = uip;
    children.push_back(std::move(ui));
}
 
void uiPiece::initFadeFunction(float timeToStart, float timeToFade, float fadeDuration) {
    float fadeRateCoefficient = 1.0f / fadeDuration;
    fadeFunction = [=] (float time_, Shader* s) {
        float alpha = glm::clamp(fadeRateCoefficient*(time_-timeToStart), 0.0f, 1.0f);
        float alpha_ = glm::clamp(fadeRateCoefficient*(timeToFade-time_),0.0f,1.0f);
        float realalpha = std::min(alpha, alpha_);
        s->use();
        s->setUniform("alpha", realalpha);
    };  
    watch.resetTime();
}
