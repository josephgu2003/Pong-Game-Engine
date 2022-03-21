//
//  WorldSubSystem.hpp
//  Pong
//
//  Created by Joseph Gu on 12/28/21.
//

#ifndef WorldSubSystem_hpp
#define WorldSubSystem_hpp

#include "Component.hpp"
#include <stdio.h>
 
class Renderer;

class World;

// like a Behavior or any other game object, but is autmatically added to each world by the game
class WorldSubSystem : public Component {
public:
    WorldSubSystem(World& w);
    virtual void drawAll(Renderer* r) {}
};

#endif /* WorldSubSystem_hpp */
