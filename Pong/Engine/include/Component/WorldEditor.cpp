//
//  WorldEditor.cpp
//  Pong
//
//  Created by Joseph Gu on 3/1/22.
//

#include "WorldEditor.hpp"
#include "Actor.hpp"
#include "uiLayout.hpp"
#include "PropFactory.hpp"
#include  "World.hpp"
#include "InputHandler.hpp"
#include <fstream>
#include "LevelSerializer.hpp"

WorldEditor::WorldEditor(Actor& a, uiLayout* ui) : Component(a) {
    std::shared_ptr<uiText> terminal_ = std::make_shared<uiText>("", 0.5, 0.9, 0.0002f, DEFAULT_LINESPACE);
    terminal = terminal_;
    ui->insertNode(terminal_);
    jsonLoaded = false;
}

void WorldEditor::tick() {
    
}

void WorldEditor::notify(const Subject &subject, GameEvent ge) {
    if (ge == KEY_PRESSED) {
        auto ih_ = static_cast<const InputHandler&>(subject);
        GLenum key = ih_.getCurrentKeyPress();
        
        if (key == GLFW_KEY_L) {
            if (jsonLoaded == false) {
                std::ifstream inStream(GAMELVLS_PATH);
                inStream >> json;
                jsonLoaded = true;
            }
            
            auto a = static_cast<Actor*>(actor);
            PropFactory pf;
            auto tree = pf.makeProp(PROP_PINE);
            tree->setPos(a->getPos()); 
            a->getWorld().insert(tree);
            
            auto tree1 = pf.makeProp(PROP_PINE); 
            tree1->setPos(a->getPos() + glm::vec3(1, 0, 2));
            a->getWorld().insert(tree1);
             
            auto tree2 = pf.makeProp(PROP_PINE);
            tree2->setPos(a->getPos() + glm::vec3(1, 0, 0));
            a->getWorld().insert(tree2);
            
            nlohmann::json& lvlSave = json.find("main").value();
            nlohmann::json& worlddata = lvlSave[0];
            nlohmann::json& arrayEntities = worlddata.find("Data").value();
            
            std::vector<float> pos0 = {tree->getPos().x, tree->getPos().y, tree->getPos().z};
            nlohmann::json t0 = {{"FactoryEnum", PROP_PINE}, {"Position", pos0}, {"Type", "Prop"}};
            arrayEntities.push_back(t0);
            
            std::vector<float> pos1 = {tree1->getPos().x, tree1->getPos().y, tree1->getPos().z};
            nlohmann::json t1 = {{"FactoryEnum", PROP_PINE}, {"Position", pos1}, {"Type", "Prop"}};
            arrayEntities.push_back(t1);
            
            std::vector<float> pos2 = {tree2->getPos().x, tree2->getPos().y, tree2->getPos().z};
            nlohmann::json t2 = {{"FactoryEnum", PROP_PINE}, {"Position", pos2}, {"Type", "Prop"}};
            arrayEntities.push_back(t2);
        }
        
        if (key == GLFW_KEY_P) {
            std::ofstream o(GAMELVLS_PATH);
            o << std::setw(4) << json << std::endl;
        }
         
        if (key == GLFW_KEY_M) {
            auto a = static_cast<Actor*>(actor);
            PropFactory pf;
            auto tree = pf.makeProp(PROP_TOWN);
            tree->setPos(a->getPos() + glm::vec3(0, 0, 0));
            a->getWorld().insert(tree);
        }
        
        if (key == GLFW_KEY_N) {
            auto a = static_cast<Actor*>(actor);
            PropFactory pf; 
            auto tree = pf.makeProp(PROP_MT);
            tree->setPos(a->getPos() + glm::vec3(0, 10, 0));
            a->getWorld().insert(tree);
        }
    }
} 
 
 
