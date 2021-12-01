//
//  MyGame.cpp
//  Pong
//
//  Created by Joseph Gu on 10/30/21.
//

#include "MyGame.hpp"
#include "HealthMeter.hpp"
#include "LifeComponent.hpp"
#include "Subject.hpp"
#include "JsonManager.hpp"
#include "uiFrame.hpp"
#include "Speech.hpp"
#include "FallingLetters.hpp"
#include "Fish.hpp"
#include "NameComponent.hpp"
#include "CombatComponent.hpp"
#include "uiText.hpp"
#include "DialogueMenu.hpp"
#include "CharacterComponent.hpp"
#include "aiDialogueAction.hpp"


#define POEM "I was asked - \"Do you have dreams?\"", "No...", "...Yes? Lost. Searching. Searching.", "Searching with colorful moonlight always overhead,","Yet my eyes were always down, scouring that dark canvas.","Too late, gaze up at the painted moon.", "A flash of inspiration, and the coldness of regret.","Is it too late? The moon is going away soon.","A brush dipped in lost dreams refound,", "But a hand still with regretfulness.","If only I had a pond, so that by its reflection,","I would have seen the moon's beauty sooner.","A brush, a canvas, a horizon","An artist dreaming of the moon."

#define CREDITS "WASD - Move", "don't touch E, R, T - old features that need new purpose", "Z - summon fish and break stuns", "X - swap world", "The boss ahead will dialogue you", "Then stun, press Z after", "Joseph Gu - Programmer", "Yirou Guo - Creative Consultant and Artist", "Jonathan Ran - Mathematical and Physics Consultant", "Matthew Ding - Deployment Help"



MyGame::MyGame() : Game() {
        
}
 

void MyGame::load() {
    setupLvlBuilder();
    loadLevel("mainmenu");
}   
 
void loadMainMenuDefaultCallbacks(InputHandler* ih) {
    ih->setCallbackforKey(GLFW_KEY_ENTER, [](Game* game){
        std::shared_ptr<uiText> ut = std::make_shared<uiText>("Loading...", -0.2, -0.5);
        game->getUI()->insertNode(ut);
        game->loadLevel("main");
    });
}
    
void loadMainGameDefaultCallbacks(InputHandler* ih) {
    ih->setCallbackforKey(GLFW_KEY_ENTER, [](Game* game){
            auto x = game->getInputHandler();
            x.setHandlerMode(KCALL_READTEXT);
            x.loadKeyCallbacks(KCALL_READTEXT);
        });
    
    ih->setCallbackforKey(GLFW_KEY_ESCAPE, [](Game* game){
        game->loadLevel("mainmenu");
    }); 

    ih->setCallbackforKey(GLFW_KEY_SPACE, [](Game* game){
            if (auto x = game->getActiveLevel().getActiveWorld()->getPlayerHero()) {
                x->jump();
            }
        }); 
 
    ih->setCallbackforKey(GLFW_KEY_Z, [](Game* game){
            if (auto ph = game->getActiveLevel().getActiveWorld()->getPlayerHero()) {
                std::shared_ptr<Ability> fish = std::make_shared<Fish>(&ph->getWorld(), ph, 18.0);
                ph->getComponent<CombatComponent>()->newAbility(fish);
            }
        });
        
    ih->setCallbackforKey(GLFW_KEY_X, [](Game* game){
        if (auto ph = game->getActiveLevel().getActiveWorld()->getPlayerHero()) {
            ph->getComponent<AnimComponent>()->playAnim("Armature|SwordSlash");
        }
        });
        
    ih->setCallbackforKey(GLFW_KEY_G, [](Game* game){
            if (auto ph = game->getActiveLevel().getActiveWorld()->getPlayerHero()) {
                std::shared_ptr<Ability> letters = std::make_shared<FallingLetters>(&ph->getWorld(), ph, 6.0);
                auto comb = ph->getComponent<CombatComponent>();
                if (comb->hasTarget()) {
                    letters->setTarget(comb->getBigTarget());
                }
                comb->newAbility(letters);
            }
        });
        
    ih->setCallbackforKey(GLFW_KEY_Q, [](Game* game){
            if (auto ph = game->getActiveLevel().getActiveWorld()->getPlayerHero()) {
                std::vector<std::string> lines = {CREDITS};
                std::shared_ptr<Ability> speech = std::make_shared<Speech>(&ph->getWorld(), ph, 6.0, lines);
                ph->getComponent<CombatComponent>()->newAbility(speech);
            }
        });
        
    ih->setCallbackforKey(GLFW_KEY_M, [](Game* game){
            if (auto ph = game->getActiveLevel().getActiveWorld()->getPlayerHero()) {
                std::vector<std::string> lines = {POEM};
                std::shared_ptr<Ability> speech = std::make_shared<Speech>(&ph->getWorld(), ph, 6.0, lines);
                ph->getComponent<CombatComponent>()->newAbility(speech);
            }
        });
        
    ih->setCallbackforKey(GLFW_KEY_Y, [](Game* game) {
            if (auto hero = game->getActiveLevel().getActiveWorld()->getPlayerHero()) {
                Actor* nearest;
                if (!hero->getWorld().getNearestActorWith(hero, CHAR, nearest)) {
                    return;
                }
                std::shared_ptr<aiDialogueAction> dialogue = std::make_shared<aiDialogueAction>(10.0f, hero, nearest);
                std::shared_ptr<DialogueMenu> u = std::make_shared<DialogueMenu>(glm::vec2(0, 0), glm::vec2(0.5,0.5), "Resources/Textures/Project-10.png", dialogue);
                hero->getComponent<CharacterComponent>()->newAction(dialogue);
                InputHandler& x = game->getInputHandler();
                game->getUI()->setActivePopup(u);
                x.addObserver(u);
                x.setHandlerMode(KCALL_MENU);
                x.loadKeyCallbacks(KCALL_MENU);
            }
        });
        
        auto shiftCall = [] (Game* g) {
            g->getInputHandler().swapCursorMode();
        };
        ih->setCallbackforKey(GLFW_KEY_LEFT_SHIFT, shiftCall);
    }
        
void loadMainGameReadTextCallbacks(InputHandler* ih) {
        ih->setCallbackforKey(GLFW_KEY_ENTER, [](Game* game){
            auto x = game->getInputHandler();
            x.setHandlerMode(KCALL_DEFAULT);
            x.dumpTextToPlayer();
            x.loadKeyCallbacks(KCALL_DEFAULT);
        });
    }

void loadMainGameMenuModeCallbacks(InputHandler* ih) {{
    ih->setCallbackforKey(GLFW_KEY_ENTER, [](Game* game){
            auto x = game->getInputHandler();
            x.setHandlerMode(KCALL_DEFAULT);
            x.loadKeyCallbacks(KCALL_DEFAULT);
            game->getUI()->deleteActivePopup();
        });
    ih->setCallbackforKey(GLFW_KEY_Y, [](Game* game) {
            auto x = game->getInputHandler();
            x.setHandlerMode(KCALL_DEFAULT);
            x.loadKeyCallbacks(KCALL_DEFAULT);
            game->getUI()->deleteActivePopup();
        });
    }
}

 
void MyGame::setupLvlBuilder() {
    GameLevelCreate makeMainMenu = [&] (Game* g) {
        GameLevel* lvl = new GameLevel(g->getRenderer(), 1);
        std::shared_ptr<uiText> ut = std::make_shared<uiText>("Press enter to begin", -0.2, 0);
        std::shared_ptr<uiFrame> uf = std::make_shared<uiFrame>(glm::vec2(-1,-1), glm::vec2(2,2), TEX_BLACK_GRADIENT);
        uf->insertChild(ut);
        g->getUI()->insertNode(uf);
        g->getInputHandler().attachCallbackSetters(KCALL_DEFAULT, loadMainMenuDefaultCallbacks);
        g->getInputHandler().loadKeyCallbacks(KCALL_DEFAULT);
        return lvl;
    };
        
    GameLevelCreate makeMain = [&] (Game* g) {
        g->getUI()->clear();
        GameLevel* lvl = new GameLevel(g->getRenderer(), 2); 
        
        JsonManager::loadGameLevel(lvl, &actorFactory, &propFactory, &particleFactory, &scriptFactory);
        
        lvl->getWorld(0).setMap("Resources/Map/landscape.png", 1024, 1024, glm::vec3(0.4, 0.002, 0.4));
           
        std::shared_ptr<HealthMeter> hm = std::make_shared<HealthMeter>();
        auto player = lvl->getActiveWorld()->getPlayerHero(); 
        player->getComponent<LifeComponent>()->addObserver(hm);
        hm->notify(*player->getComponent<LifeComponent>(), SUBJ_HP_CHANGED);
        
        std::shared_ptr<uiFrame> uf = std::make_shared<uiFrame>(glm::vec2(-0.9,0.65), glm::vec2(0.3,0.3), TEX_BLACK_GRADIENT);
        uf->insertChild(hm);
        g->getUI()->insertNode(uf);
                    
        DirectionalLight            dl2(glm::vec3(0.2,0.2,0.2),glm::vec3(0.8,0.8,0.8),glm::vec3(1.0,1.0,1.0),glm::vec3(-1,-1,0));
        std::vector<std::string> sky1;
        
        sky1.push_back("Resources/Skybox/NightStars/BlueNebular_left.jpg");
        sky1.push_back("Resources/Skybox/NightStars/BlueNebular_right.jpg");
        sky1.push_back("Resources/Skybox/NightStars/BlueNebular_top.jpg");   
        sky1.push_back("Resources/Skybox/NightStars/BlueNebular_bottom.jpg");
        sky1.push_back("Resources/Skybox/NightStars/BlueNebular_front.jpg");
        sky1.push_back("Resources/Skybox/NightStars/BlueNebular_back.jpg");
        
        lvl->getWorld(1).setWeather(dl2, 0.02, 2, glm::vec3(0), sky1);
                     
        DirectionalLight          dl(glm::vec3(0.13,0.13,0.133),glm::vec3(0.15,0.15,0.155),glm::vec3(0.3,0.3,0.3),glm::vec3(-1,-1,0));
        lvl->getWorld(0).setWeather(dl, 0.015, 2, glm::vec3(0.0,0.0,0.0), sky1);
          
        g->getInputHandler().attachCallbackSetters(KCALL_DEFAULT, loadMainGameDefaultCallbacks);
        g->getInputHandler().attachCallbackSetters(KCALL_MENU, loadMainGameMenuModeCallbacks);
        g->getInputHandler().attachCallbackSetters(KCALL_READTEXT, loadMainGameReadTextCallbacks);
        g->getInputHandler().loadKeyCallbacks(KCALL_DEFAULT); 
        return lvl; 
    };

    myLvlBuilder.insert(std::pair<std::string, GameLevelCreate>("main", makeMain));
    myLvlBuilder.insert(std::pair<std::string, GameLevelCreate>("mainmenu", makeMainMenu));
    setLevelBuilder(myLvlBuilder); 
}

  
 
