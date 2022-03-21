//
//  MyScript.cpp
//  Pong
//
//  Created by Joseph Gu on 3/17/22.
//

#include "MyScript.hpp"
#include "InputHandler.hpp"

MyScript::MyScript(World* world, std::vector<std::string> crew, float radius, bool completed, std::string scenenName, std::vector<std::string> prerequisiteScenes_, InputHandler* ih_, uiLayout* ui_) : Script(world, crew, radius, completed, scenenName, prerequisiteScenes_) {
    ih = ih_;
    ui = ui_;
    backdropDuration = 0.0f;
}

// present a two choice menu
void MyScript::openChoiceMenu(const std::string& line1, const std::string& line2) {
    auto menu = std::make_shared<uiMenu>(glm::vec2(0.0, -0.2), glm::vec2(0.5, 0.5), TEX_EMPTY);
    menu->insertChild(std::make_shared<uiText>(line1, 0.1, 0.1, DEFAULT_FONTSIZE, DEFAULT_LINESPACE, 1.0));
    menu->insertChild(std::make_shared<uiText>(line2, 0.1, -0.1, DEFAULT_FONTSIZE, DEFAULT_LINESPACE, 1.0));
    choiceMenu = menu;
    ui->insertNode(menu);
    ih->addObserver(menu);
}

// present a three choice menu
void MyScript::openChoiceMenu(const std::string& line1, const std::string& line2, const std::string& line3) {
    auto menu = std::make_shared<uiMenu>(glm::vec2(0.0, -0.2), glm::vec2(0.5, 0.5), TEX_EMPTY);
    menu->insertChild(std::make_shared<uiText>(line1, 0.1, 0.1, DEFAULT_FONTSIZE, DEFAULT_LINESPACE, 1.0));
    menu->insertChild(std::make_shared<uiText>(line2, 0.1, -0.1, DEFAULT_FONTSIZE, DEFAULT_LINESPACE, 1.0));
    menu->insertChild(std::make_shared<uiText>(line3, 0.1, -0.3, DEFAULT_FONTSIZE, DEFAULT_LINESPACE, 1.0));
    choiceMenu = menu;
    ui->insertNode(menu);
    ih->addObserver(menu);
}

int MyScript::getMenuChoiceAndClose() {
    if (auto menu = choiceMenu.lock()) { 
        int option = menu->getChosenOption();
        if (option != -1) {
            ui->removeNode(menu.get());
        } 
        return option;
    } else {
        return -1;
    }
}

void MyScript::tick() {
    if (auto black = blackBackdrop.lock()) {
        if (backdropDuration <= 0.0f) {
            ui->removeNode(black.get());
        } else {
            backdropDuration -= backdropTimer.getTime();
            backdropTimer.resetTime();
        }
    }
    
    Script::tick();
}
