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
    hidden = false;
    usingFadeFunction = false;
} 


void uiPiece::drawUI(Renderer* r) {
    r->renderGeneric(this); 
}

void uiPiece::draw(Renderer* r) {
    if (usingFadeFunction) invokeFadeFunction();
    if (!hidden) {
        drawUI(r); 
        for (auto ui = children.begin(); ui != children.end(); ui++) {
            (*ui)->draw(r);
        }
    }
}
 
void uiPiece::insertChild(const std::shared_ptr<uiPiece>& uip) {
    children.push_back(uip);
}
 
void uiPiece::initFadeFunction(float timeToStart, float timeToFade, float fadeDuration) {
    float fadeRateCoefficient = 1.0f / fadeDuration;
    fadeFunction = [=] (float time_, uiPiece* s) {
        float alpha = glm::clamp(fadeRateCoefficient*(time_-timeToStart), 0.0f, 1.0f);
        float alpha_ = glm::clamp(fadeRateCoefficient*(timeToFade-time_),0.0f,1.0f);
        float realalpha = std::min(alpha, alpha_);
        s->setUniformForAll("alpha", realalpha);
        return (realalpha == 0.0f);
    };   
    watch.resetTime();
    usingFadeFunction = true; 
}

void uiPiece::initFadeFunction(float secPerCycle) {
    fadeFunction = [=] (float time_, uiPiece* s) {
        float alpha = (1.0f + sin(2.0f * 3.1415f * time_ / secPerCycle)) / 2.0f;
        s->setUniformForAll("alpha", alpha);
        return (alpha == 0.0f);
    };
    watch.resetTime();
    usingFadeFunction = true;
}

void uiPiece::setHiddenStatus(bool status) {
    hidden = status;
}

void uiPiece::flipHiddenStatus() {
    hidden = !hidden;
}

bool uiPiece::isHidden() {
    return hidden;
}

void uiPiece::initPeriodicFadeFunction(float timeToStart, float timeToFade, float fadeDuration, float period) {
    assert(period + timeToStart > timeToFade); // is starting point + period greater than fading point?
    float fadeRateCoefficient = 1.0f / fadeDuration;
    fadeFunction = [=] (float time_, uiPiece* s) {
        float alpha = glm::clamp(fadeRateCoefficient*(fmod(time_-timeToStart, period)), 0.0f, 1.0f);
        float alpha_ = glm::clamp(fadeRateCoefficient*(std::floor((time_-timeToStart)/ period) * period + timeToFade-time_),0.0f,1.0f); 
        float realalpha = std::min(alpha, alpha_);
        s->setUniformForAll("alpha", realalpha);
        return false;
    };
    watch.resetTime();
    usingFadeFunction = true;
}
