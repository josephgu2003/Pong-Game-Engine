//
//  SoundTextManager.cpp
//  Pong
//
//  Created by Joseph Gu on 12/22/21.
//
  
#include "SubtitlesSystem.hpp"


void SubtitlesSystem::newSoundText(const std::string& text, float duration) {
    if (auto x = activeText.lock()) {
        x->setText(text);
    }
    textFrame->initFadeFunction(0.0, duration-0.05f, 0.05f);
}
  
  
void SubtitlesSystem::drawAll(Renderer* r) { 
    textFrame->draw(r);
    
} 
 
void SubtitlesSystem::tick() {
}
  
SubtitlesSystem::SubtitlesSystem(World& w) : WorldSubSystem(w) {
    std::shared_ptr<uiText> text = std::make_shared<uiText>("", -0.6, -0.8, DEFAULT_FONTSIZE * 0.8, DEFAULT_LINESPACE, 1.0);
    activeText = text;// lmfao??? 
    textFrame = std::make_unique<uiFrame>(glm::vec2(-0.7, -0.9), glm::vec2(1.5,0.23), "Resources/GlyphsAndUI/subtitles.png");
   // textFrame = std::make_unique<uiFrame>(glm::vec2(-1.0, -1.0), glm::vec2(2.0,2.0), TEX_EMPTY);
    textFrame->insertChild(text);
    textFrame->setHiddenStatus(true);
} 
    
 
