//
//  ManaMeter.hpp
//  Pong
//
//  Created by Joseph Gu on 12/3/21.
//

#ifndef ManaMeter_hpp
#define ManaMeter_hpp

#include <stdio.h>
#include "uiPiece.hpp"
#include "Observer.hpp"

class ManaMeter : public uiPiece, public Observer { 
public:
    ManaMeter(glm::vec2 pos, glm::vec2 scaling);
    virtual void notify(const Subject& s, GameEvent ge) override;
};
#endif /* ManaMeter_hpp */
 
