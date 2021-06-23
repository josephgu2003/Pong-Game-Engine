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
    std::vector<std::string> endLeftLines = {"'Because I must try. At the very least'",
"But you failed, and now you are vulnerable.", "I won't let you leave here again."};
    DialogueTree* endLeft = new DialogueTree();
    *endLeft = {endLeftLines, nullptr, nullptr};
    
    std::vector<std::string> endRightLines = { "'I don't know'","Poor bird, lost its will to fly.", "I won't let you leave here again."};
    DialogueTree* endRight = new DialogueTree();
    *endRight = {endRightLines, nullptr, nullptr};

    std::vector<std::string> mainLines1 = {"'Why are you here?'","...","(And then...)","In a faraway land, there lives a bird.",
        "Every twelves months, that bird spreads it's wings and flies North",
        "Seeking company, adventure, and summer.",
        "But each year without fail, that bird returns",
        "Needing shelter from the winters of that land of snow.",
        " Why don't you stay here forever?",
        " This is a world of black and white",
        " Win, lose, good, evil.",
        " Why do you try to fly North? (Type A/B)"};
    
    std::vector<std::string> mainLines2 = {"'I've returned'","...","(And then...)","In a faraway land, there lives a bird.",
        "Every twelves months, that bird spreads it's wings and flies North",
        "Seeking company, adventure, and summer.",
        "But each year without fail, that bird returns",
        "Needing shelter from the winters of that land of snow.",
        " Why don't you stay here forever?",
        " This is a world of black and white",
        " Win, lose, good, evil.",
        " Why do you try to fly North? (Type A/B)"};
    
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

/**
 Dialogue::Dialogue(World* world_, Actor* actor_, float duration_) : Ability::Ability(world_, actor_, duration_) {
     std::vector<std::string> endLeftLines = {"'Because I must try. At the very least'",
 "But you failed, and now you are vulnerable.", "I won't let you leave here again."};
     DialogueTree* endLeft = new DialogueTree();
     *endLeft = {endLeftLines, nullptr, nullptr};
     
     std::vector<std::string> endRightLines = { "'I don't know'","Poor bird, lost its will to fly.", "I won't let you leave here again."};
     DialogueTree* endRight = new DialogueTree();
     *endRight = {endRightLines, nullptr, nullptr};

     std::vector<std::string> mainLines = {"In a faraway land, there lives a bird.",
         "Every twelves months, that bird spreads it's wings and flies North",
         "Seeking company, adventure, and summer.",
         "But each year without fail, that bird returns",
         "Needing shelter from the winters of that land of snow.",
         " Why don't you stay here forever?",
         " This is a world of black and white",
         " Win, lose, good, evil.",
         " Why do you try to fly North? (Type A/B)"};
     dialogueTree = {mainLines, endLeft, endRight};
 }
 */

