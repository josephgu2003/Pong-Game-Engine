//
//  InputCallback.cpp
//  Pong
//
//  Created by Joseph Gu on 10/24/21.
//

#include "InputCallback.hpp"
#include "Game.hpp"

InputCallback::InputCallback(keyCallback exec_, keyCallback undo_) {
    exec = exec_;
    undo = undo_;
    toggled = false;
    enableToggling = false;
}

void InputCallback::execute(Game* g) {
    if (enableToggling && toggled) {
        undoEffects(g);
        toggled = false;
        return;
    }
    exec(g);
    toggled = true; 
}

void InputCallback::undoEffects(Game* g) {
    undo(g);
}

void InputCallback::enableToggle()
{
    enableToggling = true;
}
