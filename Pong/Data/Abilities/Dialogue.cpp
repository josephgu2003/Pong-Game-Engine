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
"But you fell, and now you are vulnerable.", "It would be better to stay here."};
    DialogueTree* endLeft = new DialogueTree();
    *endLeft = {endLeftLines, nullptr, nullptr};
    
    std::vector<std::string> endRightLines = { "'Some people want to see some new colors.'","'That's why I must go'", "Figure: Colors bring choices. Choices bring pain.","Figure: It would be better to stay here."};
    DialogueTree* endRight = new DialogueTree();
    *endRight = {endRightLines, nullptr, nullptr};

    std::vector<std::string> mainLines1 = {"'Oh?'","...", "Figure: Come here.", "Figure: Look to the horizon.",
        "Figure: Every year, the albatross spreads its wings and flies North",
        "Figure: Seeking that mysterious and unknown adventure.",
        "Figure: But every year without fail, the albatross returns",
        "Figure: Needing shelter from the winters of that land of snow.",
        "Figure: Stay here in this world of black and white. Why leave? (Type A/B)"};
    
    DialogueTree* mainbody1 = new DialogueTree();
    *mainbody1 = {mainLines1, endLeft, endRight};
    
    std::vector<std::string> firstLine = {"A lone figure in the distance. (Type A/B)"};
    
    dialogueTree = {firstLine, mainbody1, mainbody1};
    currentTree = &dialogueTree;
}

void Dialogue::call(Game* game_) {
    game = game_;
    game->newDialogue(*this);
    on = true;
    paused = false;
   // world->setActiveText(getCurrentLines().at(counter));
    world->setActiveText(currentTree->lines.at(step));
    activeText = currentTree->lines.at(step);
    step++;
}

void Dialogue::tick() {
    elapsetime+=glfwGetTime();

    if (step == currentTree->lines.size()) {
        if (currentTree->left == nullptr && currentTree->right == nullptr) {
            on = false;
            return;
        }
        game->setActionScheme(3);
        step = 0;
        paused = true;
        return;
    }
    if (elapsetime > 3.0 && !paused) {
     //   world->setActiveText(getCurrentLines().at(counter));
        world->setActiveText(currentTree->lines.at(step));
        activeText = currentTree->lines.at(step);
        step++;
        elapsetime = 0;
    }
}

void Dialogue::branch(int direction) {
    if(direction==0) {currentTree = currentTree->left;}
    if (direction==1) {currentTree = currentTree->right;}
    paused = false;
}

std::string Dialogue::getActiveText() {
    return activeText;
}
