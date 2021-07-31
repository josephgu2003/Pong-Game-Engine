//
//  Dialogue.cpp
//  Pong
//
//  Created by Joseph Gu on 6/17/21.
//

#include "Dialogue.hpp"
#include "Game.hpp"
#include "World.hpp"
#include "AssetManager.hpp"
#include "Renderer.hpp"

Dialogue::Dialogue(World* world_, Actor* actor_, float duration_, int id) : Ability::Ability(world_, actor_, duration_) {
    AssetManager::loadDialogue(this, id);
    currentTree = dialogueTree;
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
 
void Dialogue::setDialogueTree(DialogueTree* tree) {
    int i = 0;
    dialogueTree=tree;
}
