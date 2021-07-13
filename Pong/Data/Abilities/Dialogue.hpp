//
//  Dialogue.hpp
//  Pong
//
//  Created by Joseph Gu on 6/17/21.
//

#ifndef Dialogue_hpp
#define Dialogue_hpp

#include "Ability.hpp"
#include <stdio.h>
#include <vector>

#define LEFT 0
#define RIGHT 1

class Renderer;

struct DialogueTree {
    std::vector<std::string> lines;
    DialogueTree* left;
    DialogueTree* right;
};

class Dialogue : public Ability {
private:
    bool paused;
    DialogueTree dialogueTree;
    DialogueTree* currentTree;
    Renderer* renderer;
    std::string activeText;
    float elapsetime = 0;
public:
    Dialogue(World* world_, Actor* actor_, float duration_);
    virtual void call(Game* game);
    virtual void tick();
    void branch(int direction);
    std::string getActiveText();
};
#endif /* Dialogue_hpp */




// In a faraway land, there lives a bird.
// Every twelves months, that bird spreads it's wings and flies North
// Seeking company, adventure, and color.
// But every year without fail, that bird returns South in the winter
// For it cannot survive in that land of snow.
// Why don't you stay here forever?
// This is a world of black and white
// Win, lose, good, evil.
// Why chase after dreams? Why enter that world of color?

// "Because I must try. At the very least."
// "I don't know."

// a: But you failed, and now you are vulnerable. I won't let you leave here again.
// b: Poor bird, lost its will to fly. I won't let you leave here again.

