//
//  SoundTextManager.cpp
//  Pong
//
//  Created by Joseph Gu on 12/22/21.
//

#include "SoundTextManager.hpp"
 

void SoundTextManager::newSoundText(const std::string& text, const glm::vec3& pos, float duration) {
    soundText.reset();
    soundText = std::make_unique<SoundText>(text, pos, duration);
    update = true;
}
  
void SoundTextManager::updateActiveText(const glm::vec3& pos) {
    std::string s = "";
    if (soundText.get()) {
        s = soundText->text;
    }
    if (auto x = activeText.lock()) {
        x->setText(s);
    }  
    update = false;
} 

void SoundTextManager::drawAll(Renderer* r) {
    textFrame->draw(r);
} 

void SoundTextManager::tick(const glm::vec3& pos) {
    if (soundText.get()) {
        soundText->duration -= (float) globalTime.getTime();
         
        float alpha = soundText->duration / soundText->maxDuration; //HAHAHA USELESS
        
        if (alpha < 0.1 || alpha > 0.9) {
            if (alpha > 0.9) alpha = 1.0-alpha;
            if (alpha < 0.0f) alpha = 0.0f;
            alpha *= 10.0f; 
            if (auto x = activeText.lock()) {
                x->getShader()->use();
                x->getShader()->setUniform("alpha", alpha);
            }
            textFrame->getShader()->use();
            textFrame->getShader()->setUniform("alpha", alpha);
        }
        if (soundText->duration <= 0.0f) {
            soundText.reset();
            update = true;  
        }
    }
     
    globalTime.resetTime();
    
    if (update) {
        updateActiveText(pos);
    } 
}

SoundTextManager::SoundTextManager() { 
    update = false; 
    std::shared_ptr<uiText> text = std::make_shared<uiText>("", -0.5, -0.8, DEFAULT_FONTSIZE, DEFAULT_LINESPACE, 1.0);
    activeText = text;// lmfao??? 
    textFrame = std::make_unique<uiFrame>(glm::vec2(-0.7, -0.9), glm::vec2(1.5,0.23), TEX_BLACK_GRADIENT);
    textFrame->insertChild(text);
    textFrame->getShader()->use();
    textFrame->getShader()->setUniform("alpha", 0.0f);
} 
  
