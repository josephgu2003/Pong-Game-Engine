//
//  WorldEditor.cpp
//  Pong
//
//  Created by Joseph Gu on 3/1/22.
//

#include "WorldEditor.hpp"
#include "Actor.hpp"
#include "uiLayout.hpp"
#include "PropFactory.hpp"
#include  "World.hpp"
#include "InputHandler.hpp"

WorldEditor::WorldEditor(Actor& a, uiLayout* ui) : Component(a) {
    std::shared_ptr<uiText> terminal_ = std::make_shared<uiText>("", 0.5, 0.9, 0.0002f, DEFAULT_LINESPACE);
    terminal = terminal_;
    ui->insertNode(terminal_);
}

void WorldEditor::tick() {
    
}

void WorldEditor::notify(const Subject &subject, GameEvent ge) {
    if (ge == KEY_PRESSED) {
        auto ih_ = static_cast<const InputHandler&>(subject);
        GLenum key = ih_.getCurrentKeyPress();
        
        if (key == GLFW_KEY_L) {
            auto a = static_cast<Actor*>(actor);
            PropFactory pf;
            auto tree = pf.makeProp(PROP_LIGHTRAY); 
            tree->setPos(a->getPos());
            a->getWorld().insert(tree);
        }
    }
} 
