//
//  Dialogue.cpp
//  Pong
//
//  Created by Joseph Gu on 6/17/21.
//

#include "Dialogue.hpp"
#include "Game.hpp" 
#include "World.hpp"
#include "Renderer.hpp"
#include "JsonManager.hpp"
#include "SubtitlesSystem.hpp"
 

Dialogue::Dialogue(World* world_, Actor* actor_, float duration_, int id) : Ability::Ability(world_, actor_, duration_) {
    JsonManager::loadDialogue(this, id);
    currentTree = dialogueTree;
}

void Dialogue::call() {
    on = true;
    paused = false;
   // world->setActiveText(getCurrentLines().at(counter));
world->getComponent<SubtitlesSystem>()->newSoundText(currentTree->lines.at(step), 5.0f);
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
        //game->getInputHandler().setActionScheme(3);
        step = 0;
        paused = true;
        return;
    }
    if (elapsetime > 3.0 && !paused) {
     //   world->setActiveText(getCurrentLines().at(counter));
        world->getComponent<SubtitlesSystem>()->newSoundText(currentTree->lines.at(step), 5.0f);
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
 
void Dialogue::setDialogueTree(DialogueTree* tree) {
    dialogueTree=tree;
}
