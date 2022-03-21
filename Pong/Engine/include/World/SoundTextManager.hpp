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
class Renderer;
struct SoundText {
    std::string text;
    glm::vec3 pos;
    float duration;
    float maxDuration;
    SoundText(const std::string& text_, const glm::vec3& pos_, float duration_) {
        text = text_;
        pos = pos_;
        duration = duration_;
        maxDuration = duration_;
    }
};

class SoundTextManager {
private:
    std::weak_ptr<uiText> activeText;
    std::unique_ptr<uiFrame> textFrame;
    std::unique_ptr<SoundText> soundText;
    
    bool update;
    Watch globalTime;
    void updateActiveText(const glm::vec3& pos);
public:
    SoundTextManager();
    void newSoundText(const std::string& text, const glm::vec3& pos, float duration);
    void drawAll(Renderer* r);
    void tick(const glm::vec3& pos);
};

#endif /* SoundTextManager_hpp */
