//
//  Dialogue.cpp
//  Pong
//
//  Created by Joseph Gu on 6/17/21.
//

#include "Dialogue.hpp"
#include "Game.hpp"
#include "World.hpp"


Dialogue::Dialogue(World* world_, Actor* actor_, float duration_) : Ability::Ability(world_, actor_, duration_) {
    std::vector<std::string> endLeftLines = {"'I know that to fly means to risk falling'", "'But I have to try, at the very least.'",
"But you failed, and now you are vulnerable.", "I won't let you leave here again."};
    DialogueTree* endLeft = new DialogueTree();
    *endLeft = {endLeftLines, nullptr, nullptr};
    
    std::vector<std::string> endRightLines = { "'Black, white. Some people want to see a bit of color.'","'That's why I must go'", "You are a fool.","I won't let you leave here again."};
    DialogueTree* endRight = new DialogueTree();
    *endRight = {endRightLines, nullptr, nullptr};

    std::vector<std::string> mainLines1 = {"'Why are you here?'","...","(And then...)","There's a species of bird that lives near the sea.",
        "Every year, that bird spreads its wings and flies North",
        "Seeking company, adventure, and summer.",
        "But every year without fail, that bird returns",
        "Needing shelter from the winters of that land of snow.",
        " Why don't you stay here forever?",
        " This is a world of black and white",
        " Win, lose, good, evil. No wondering, no pain. (Type A/B)"};
    
    std::vector<std::string> mainLines2 = {"'I've returned'","...","(And then...)","There's a species of bird that lives near the sea.",
        "Every year, that bird spreads its wings and flies North",
        "Seeking company, adventure, and summer.",
        "But every year without fail, that bird returns",
        "Needing shelter from the winters of that land of snow.",
        " Why don't you stay here forever?",
        " This is a world of black and white",
        " Win, lose, good, evil. No wondering, no hurt. (Type A/B)"};
    
    DialogueTree* mainbody1 = new DialogueTree();
    *mainbody1 = {mainLines1, endLeft, endRight};
    DialogueTree* mainbody2 = new DialogueTree();
    *mainbody2 = {mainLines2, endLeft, endRight};
    
    std::vector<std::string> firstLine = {"A lone figure in the distance. (Type A/B)"};
    
    dialogueTree = {firstLine, mainbody1, mainbody2};
    currentTree = &dialogueTree;
    
}

void Dialogue::call(Game* game_) {
    game = game_;
    game->newDialogue(*this);
    on = true;
    paused = false;
   // world->setActiveText(getCurrentLines().at(counter));
    world->setActiveText(currentTree->lines.at(counter));
    counter++;
}

void Dialogue::tick() {
    elapsetime+=glfwGetTime();

    if (counter == currentTree->lines.size()) {
        if (currentTree->left == nullptr && currentTree->right == nullptr) {
            on = false;
            return;
        }
        game->setActionScheme(3);
        counter = 0;
        paused = true;
        return;
    }
    if (elapsetime > 4.3 && !paused) {
     //   world->setActiveText(getCurrentLines().at(counter));
        world->setActiveText(currentTree->lines.at(counter));
        counter++;
        elapsetime = 0;
    }
}

void Dialogue::branch(int direction) {

    
    if(direction==0) {currentTree = currentTree->left;}
    if (direction==1) {currentTree = currentTree->right;}
    paused = false;
}
