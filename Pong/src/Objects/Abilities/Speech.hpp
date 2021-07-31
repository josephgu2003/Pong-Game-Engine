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
    Renderer* renderer = NULL;
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
    float elapsetime;
    float duration;
    bool running;
public:
    Speech(World* world_, Actor* actor_, float duration_,std::vector<std::string> strings);
    virtual ~Speech() override;
    virtual void call(Game* game) override;
    virtual void tick() override;
};
    
#endif /* Speech_hpp */


// Yirou, I probably shouldn't have asked you about your art philosophy before I explained my music first. I'm reading off a script now, because I'm terrible with words and speaking. Which is why recording this is probably good practice.
//Why I play music? Well, I'm a creative person, and I like making something beautiful. But more than that, sometimes people have things on their mind that they either can't express through words, or they simply don't have the courage to say. For me, music became my second and better voice. Sometimes people break down a bit and they can't play music anymore. That was me the whole year until now. Thanks for appreciating my music. I think I'm gonna go shopping for new strings on Amazon with that hundred dollars from last night now.
