//
//  SoundTextManager.hpp
//  Pong
//
//  Created by Joseph Gu on 12/22/21.
//

#ifndef SoundTextManager_hpp
#define SoundTextManager_hpp

#include <stdio.h>
#include <string>
#include <memory>
#include <vector>
#include "uiFrame.hpp"
#include "uiText.hpp"
#include "WorldSubSystem.hpp"

class Renderer;

// should really separate this into notif and subtitle system
class SubtitlesSystem : public WorldSubSystem   {
private:
    std::weak_ptr<uiText> activeText;
    std::unique_ptr<uiFrame> textFrame;

public:
    SubtitlesSystem(World& w);
    void newSoundText(const std::string& text, float duration);
    void drawAll(Renderer* r) override;
    void tick() override;
};

#endif /* SoundTextManager_hpp */
