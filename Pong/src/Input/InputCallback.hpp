//
//  InputCallback.hpp
//  Pong
//
//  Created by Joseph Gu on 10/24/21.
//

#ifndef InputCallback_hpp
#define InputCallback_hpp

#include <stdio.h>

class Game;

typedef void (*keyCallback) (Game*);


class InputCallback {
    keyCallback exec;
    keyCallback undo;
    bool toggled;
    bool enableToggling;
public:
    InputCallback(keyCallback exec, keyCallback undo);
    void enableToggle();
    void execute(Game* g);
    void undoEffects(Game* g);
};

#endif /* InputCallback_hpp */
