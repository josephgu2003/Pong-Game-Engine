//
//  SScriptOne.hpp
//  Pong
//
//  Created by Joseph Gu on 7/5/21.
//

#ifndef SScriptOne_hpp
#define SScriptOne_hpp

#include <stdio.h>
#include "SceneScript.hpp"

class SScriptOne : public SceneScript {
public:
    virtual void tick() override;
    virtual void init() override;
};

#endif /* SScriptOne_hpp */
