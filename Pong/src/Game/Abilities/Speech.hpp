//
//  Speech.hpp
//  Pong
//
//  Created by Joseph Gu on 6/7/21.
//

#ifndef Speech_hpp
#define Speech_hpp

#include <stdio.h>
#include "Ability.hpp"
#include "World.hpp"
#include "Actor.hpp"
#include "Renderer.hpp"
#include <vector>
#include <string>


class Speech: public Ability{
    Renderer* renderer;
   /** std::vector<std::string> lines = {"I was asked - \"Do you have dreams?\"",
        "No...",
        "...Yes? Lost. Searching. Searching.",
         "Searching with colorful moonlight always overhead,",
         "Yet my eyes were always down, scouring that dark canvas.",
         "Too late, gaze up at the painted moon.",
         "A flash of inspiration, and the coldness of regret.",
         "Is it too late? The moon is going away soon.",
         "A brush dipped in lost dreams refound,",
         "But a hand still with regretfulness.",
        "If only I had a pond, so that by its reflection,",
        "I would have seen the moon's beauty sooner.",
        "A brush, a canvas, a horizon",
        "An artist dreaming of the moon."
    };**/
    
 //   std::vector<std::string> lines = { "Oh, so you are approaching me?", "Joseph Gu - Lead Programmer", "Yirou Guo - Artistic Consultant and Artist", "Jonathan Ran - Mathematical and Physics Consultant", "Joseph's Dad - Programming and Math Consultant"};
    std::vector<std::string> lines;
    int counter;
    float elapsetime;
    bool running;
public:
    Speech(World* world_, Actor* actor_, float duration_,std::vector<std::string> strings);
    virtual void call(void* pointer);
    virtual void tick();
};
    
#endif /* Speech_hpp */
