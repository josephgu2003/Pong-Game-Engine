//
//  SoundTextManager.hpp
//  Pong
//
//  Created by Joseph Gu on 12/22/21.
//

#ifndef SoundTextManager_hpp
#define SoundTextManager_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Watch.hpp"
#include <vector>
#include "uiFrame.hpp"
#include "uiText.hpp"
#include "WorldSubSystem.hpp"

class Renderer;
struct SoundText {
    std::string text;
    float duration;
    float maxDuration;
    SoundText(const std::string& text_, float duration_) {
        text = text_;
        duration = duration_;
        maxDuration = duration_;
    }
}; 
  
class SubtitlesSystem : public WorldSubSystem {
private:
    std::weak_ptr<uiText> activeText;
    std::unique_ptr<uiFrame> textFrame;
    std::unique_ptr<SoundText> soundText;
    
    bool update;
    Watch globalTime;
    void updateActiveText();
public:
    SubtitlesSystem(World& w);
    void newSoundText(const std::string& text, float duration);
    void drawAll(Renderer* r) override;
    void tick() override;
};

#endif /* SoundTextManager_hpp */
