//
//  MyScript.hpp
//  Pong
//
//  Created by Joseph Gu on 3/17/22.
//

#ifndef MyScript_hpp 
#define MyScript_hpp

#include <stdio.h>
#include "Script.hpp"
#include "uiFrame.hpp"
#include "uiMenu.hpp"
#include "uiLayout.hpp"
#include "uiText.hpp"

class InputHandler;

class MyScript : public Script {
private:
    uiLayout* ui;
    InputHandler* ih;
    std::weak_ptr<uiMenu> choiceMenu;
    std::weak_ptr<uiFrame> blackBackdrop; 
    Watch backdropTimer;
    float backdropDuration;

protected:
    // present a two choice menu
    void openChoiceMenu(const std::string& line1, const std::string& line2);
    
    // present a three choice menu
    void openChoiceMenu(const std::string& line1, const std::string& line2, const std::string& line3);
    
    // gets menu choice, delete menu if choice found
    int getMenuChoiceAndClose();
    
    // fade to a text sequence
    template <typename ...T>
    void fadeToTextSequence(float fadeTime, const T& ...args) {
        auto black = std::make_shared<uiFrame>(glm::vec2(-1.0, -1.0), glm::vec2(2.0,2.0), "Resources/GlyphsAndUI/blacksquare.png");
        
        float timeToDisplayText = 0.0f;
        
        fadeToTextSequenceHelper(fadeTime, 0, timeToDisplayText, black, args...);
           
        black->initFadeFunctionThisOnly(0.0, 2.0 * fadeTime + timeToDisplayText, fadeTime);
          
        backdropDuration = 2.0 * fadeTime + timeToDisplayText; 
        backdropTimer.resetTime();
        blackBackdrop = black; 
        ui->insertNode(black);
    } 
     
    // fade to a text sequence (helper)
    template <typename ...T>
    void fadeToTextSequenceHelper(float startupDelay, int textIndex, float& timeToDisplayText, std::shared_ptr<uiFrame>& backdrop, const std::string& line, const T& ...args) {
        auto ut = std::make_shared<uiText>(line, -0.65, -0.1, DEFAULT_FONTSIZE, DEFAULT_LINESPACE * 2.0, 1.0);
        ut->initFadeFunction(startupDelay + textIndex * 4.0f, startupDelay + 3.0f + textIndex * 4.0f, 0.5f);
        backdrop->insertChild(ut);
        timeToDisplayText = timeToDisplayText + 4.0f;
        fadeToTextSequenceHelper(startupDelay, textIndex + 1, timeToDisplayText, backdrop, args...);
    }
     
    // fade to a text sequence (helper, base case)
    template <>
    void fadeToTextSequenceHelper(float startupDelay, int textIndex, float& timeToDisplayText, std::shared_ptr<uiFrame>& backdrop, const std::string& line) {
        auto ut = std::make_shared<uiText>(line, -0.65, -0.1, DEFAULT_FONTSIZE, DEFAULT_LINESPACE * 2.0, 1.0);
        ut->initFadeFunction(startupDelay + textIndex * 4.0f, startupDelay + 3.0f + textIndex * 4.0f, 0.5f);
        backdrop->insertChild(ut);
        timeToDisplayText = timeToDisplayText + 4.0f;
    }
     

public:
    MyScript(World* world, std::vector<std::string> crew, float radius, bool completed, std::string scenenName, std::vector<std::string> prerequisiteScenes_, InputHandler* ih, uiLayout* ui);
    void tick() override final;
};

#endif /* MyScript_hpp */
