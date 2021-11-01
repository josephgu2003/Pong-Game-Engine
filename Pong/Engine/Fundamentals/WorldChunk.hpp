//
//  WorldChunk.hpp
//  Pong
//
//  Created by Joseph Gu on 10/30/21.
//

#ifndef WorldChunk_hpp
#define WorldChunk_hpp

#include <stdio.h>
#include "Renderable.hpp" 
class Renderer;

class MapChunk : public Renderable {
    public :
    MapChunk(int chunkX, int chunkY); 
    virtual void draw(Renderer* r);
};
  

#endif /* WorldChunk_hpp */
