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
#include "ManaMeter.hpp" 
#include "DevPosTracker.hpp"
#include "SwordWorld.hpp"

#define POEM "I was asked - \"Do you have dreams?\"", "No...", "...Yes? Lost. Searching. Searching.", "Searching with colorful moonlight always overhead,","Yet my eyes were always down, scouring that dark canvas.","Too late, gaze up at the painted moon.", "A flash of inspiration, and the coldness of regret.","Is it too late? The moon is going away soon.","A brush dipped in lost dreams refound,", "But a hand still with regretfulness.","If only I had a pond, so that by its reflection,","I would have seen the moon's beauty sooner.","A brush, a canvas, a horizon","An artist dreaming of the moon."


MyGame::MyGame() : Game() {

}
 

void MyGame::load() {
    scriptFactory.setUI(ui);
    setupLvlBuilder();
    loadLevel("mainmenu");
}
 
void loadMainMenuDefaultCallbacks(InputHandler* ih) {
    ih->setHandlerMode(KCALL_DEFAULT);
    ih->setOneTapCallback(GLFW_KEY_ENTER, [](Game* game){
        std::shared_ptr<uiText> ut = std::make_shared<uiText>("Loading...", -0.2, -0.5, DEFAULT_FONTSIZE, DEFAULT_LINESPACE);
        game->getUI()->insertNode(ut);
        game->loadLevel("main");
    });
} 
      
void loadMainGameDefaultCallbacks(InputHandler* ih) {
    ih->setHandlerMode(KCALL_DEFAULT);
    ih->setOneTapCallback(GLFW_KEY_ENTER, [](Game* game){
            InputHandler& x = game->getInputHandler();
            x.setHandlerMode(KCALL_READTEXT);
        });
    
    ih->setOneTapCallback(GLFW_KEY_K, [](Game* game){
        game->loadLevel("mainmenu");
    });  

    ih->setOneTapCallback(GLFW_KEY_SPACE, [](Game* game){
            if (auto x = game->getPlayerHero()) {
                x->jump();
            }
        });
    ih->setOneTapCallback(GLFW_KEY_W, [](Game* game){
            if (auto x = game->getPlayerHero()) x->getComponent<AnimComponent>()->playDefault();
    });
    ih->setOneTapCallback(GLFW_KEY_S, [](Game* game){
            if (auto x = game->getPlayerHero()) x->getComponent<AnimComponent>()->playDefault();
    });
    ih->setOneTapCallback(GLFW_KEY_A, [](Game* game){
            if (auto x = game->getPlayerHero()) x->getComponent<AnimComponent>()->playDefault();
    });
    ih->setOneTapCallback(GLFW_KEY_D, [](Game* game){
            if (auto x = game->getPlayerHero()) x->getComponent<AnimComponent>()->playDefault();
    });
    
    ih->setContinuousCallback(GLFW_KEY_W, [](Game* game){
        if (auto x = game->getPlayerHero()) x->posDir(0.09);
    });
    ih->setContinuousCallback(GLFW_KEY_S, [](Game* game){
        if (auto x = game->getPlayerHero()) x->posDir(-0.09);
    });
    ih->setContinuousCallback(GLFW_KEY_A, [](Game* game){
        if (auto x = game->getPlayerHero()) x->posRight(-0.09);
    });
    ih->setContinuousCallback(GLFW_KEY_D, [](Game* game){ 
        if (auto x = game->getPlayerHero()) x->posRight(0.09);
    }); 
 
    ih->setOneTapCallback(GLFW_KEY_Z, [](Game* game){
            if (auto ph = game->getPlayerHero()) {
                std::shared_ptr<Ability> fish = std::make_shared<Fish>(&ph->getWorld(), ph, 18.0);
                ph->getComponent<CombatComponent>()->newAbility(fish);
            }
        });
        
    ih->setOneTapCallback(GLFW_KEY_X, [](Game* game){
        if (auto ph = game->getPlayerHero()) { 
            ph->getComponent<AnimComponent>()->playAnim("Attack", false);
        }
        });
        
    ih->setOneTapCallback(GLFW_KEY_G, [](Game* game){
            if (auto ph = game->getPlayerHero()) {
                std::shared_ptr<Ability> letters = std::make_shared<FallingLetters>(&ph->getWorld(), ph, 6.0);
                auto comb = ph->getComponent<CombatComponent>();

                comb->newAbility(letters);
            }
        });
    
    ih->setOneTapCallback(GLFW_KEY_C, [](Game* game){ 
            if (auto ph = game->getPlayerHero()) {
                std::shared_ptr<Ability> letters = std::make_shared<SwordWorld>(&ph->getWorld(), ph, 6.0);
                auto comb = ph->getComponent<CombatComponent>();
    
                comb->newAbility(letters);
            }
        });


        
    ih->setOneTapCallback(GLFW_KEY_Y, [](Game* game) {
            if (auto hero = game->getPlayerHero()) {
                Actor* nearest;
                if (!hero->getWorld().getNearestActorWith(hero, CHAR, nearest)) {
                    return;
                }
                std::shared_ptr<aiDialogueAction> dialogue = std::make_shared<aiDialogueAction>(10.0f, hero, nearest);
                std::shared_ptr<DialogueMenu> u = std::make_shared<DialogueMenu>(glm::vec2(0, 0), glm::vec2(0.5,0.5), "Resources/GlyphsAndUI/Project-10.png", dialogue);
                hero->getComponent<CharacterComponent>()->newAction(dialogue);
                InputHandler& x = game->getInputHandler();
                game->getUI()->setActivePopup(u);
                x.addObserver(u);
                x.setHandlerMode(KCALL_MENU);
            }
        });
        
        auto shiftCall = [] (Game* g) {
            g->getInputHandler().swapCursorMode();
        };
        ih->setOneTapCallback(GLFW_KEY_LEFT_SHIFT, shiftCall);
    }
        
void loadMainGameReadTextCallbacks(InputHandler* ih) {
    ih->setHandlerMode(KCALL_READTEXT);
        ih->setOneTapCallback(GLFW_KEY_ENTER, [](Game* game){
            InputHandler& x = game->getInputHandler();
            x.setHandlerMode(KCALL_DEFAULT);
            x.dumpTextToPlayer();
        });
    }

void loadMainGameMenuModeCallbacks(InputHandler* ih) {{
    ih->setHandlerMode(KCALL_MENU);
    ih->setOneTapCallback(GLFW_KEY_ENTER, [](Game* game){
        InputHandler& x = game->getInputHandler();
            x.setHandlerMode(KCALL_DEFAULT);
            game->getUI()->deleteActivePopup();
        });
    ih->setOneTapCallback(GLFW_KEY_Y, [](Game* game) {
        InputHandler& x = game->getInputHandler();
            x.setHandlerMode(KCALL_DEFAULT);
            game->getUI()->deleteActivePopup(); 
        });
    } 
}

GameLevel* makeMainMenu(Game* g) {
    g->getUI()->clear();
    GameLevel* lvl = new GameLevel(g->getRenderer(), 1);
    auto ut = std::make_shared<uiText>("Press enter to begin", -0.2, 0,DEFAULT_FONTSIZE, DEFAULT_LINESPACE);
    auto uf = std::make_shared<uiFrame>(glm::vec2(-1,-1), glm::vec2(2,2), TEX_BLACK_GRADIENT);
    uf->insertChild(ut);
    g->getUI()->insertNode(uf);
    
    InputHandler& ih = g->getInputHandler();
    ih.clear();
    loadMainMenuDefaultCallbacks(&ih);
    ih.setHandlerMode(KCALL_DEFAULT);
    return lvl;
}
 
void MyGame::setupLvlBuilder() {

    GameLevelCreate makeMain = [&] (Game* g) {
        GameLevel* lvl = new GameLevel(g->getRenderer(), 2);
        auto ui = g->getUI();
        ui->clear();
         
        JsonManager::loadGameLevel(lvl, &actorFactory, &propFactory, &particleFactory, &scriptFactory);
        
        lvl->getWorld(0).setMap("Resources/Map/landscape.png", glm::vec3(0.4, 0.001, 0.4));
            
        std::shared_ptr<HealthMeter> hm = std::make_shared<HealthMeter>();
        auto player = lvl->getActiveWorld()->getPlayerHero(); 
        auto playerlife = player->getComponent<LifeComponent>();
        playerlife->addObserver(hm);
        hm->notify(*playerlife, SUBJ_HP_CHANGED);
         
        auto uf = std::make_shared<uiFrame>(glm::vec2(-0.90,0.65), glm::vec2(0.2,0.3), TEX_BLACK_GRADIENT);
        uf->insertChild(hm);
        ui->insertNode(uf);
        
        auto manaMeter = std::make_shared<ManaMeter>(glm::vec2(-0.87, 0.4), glm::vec2(0.13,0.2));
        playerlife->addObserver(manaMeter);
        manaMeter->notify(*playerlife, SUBJ_MANA_CHANGED);
          
        auto ufmana = std::make_shared<uiFrame>(glm::vec2(-0.9,0.35), glm::vec2(0.2,0.3), TEX_BLACK_GRADIENT);
        ufmana->insertChild(manaMeter);
        ui->insertNode(ufmana);
          
        auto dpt = std::make_shared<DevPosTracker>(0.5,0.8, 0.5,0.5);
        player->addObserver(dpt);   
        ui->insertNode(dpt);
                              
        DirectionalLight            dl2(glm::vec3(0.175,0.16,0.2),glm::vec3(0.43,0.39,0.50),glm::vec3(1.3,1.3,1.4),glm::vec3(-1,-1,0)); 
                  
        std::vector<std::string> sky1 = { 
            "Resources/Skybox/NightStars/BlueNebular_left.jpg",
            "Resources/Skybox/NightStars/BlueNebular_right.jpg",
            "Resources/Skybox/NightStars/BlueNebular_top.jpg",
            "Resources/Skybox/NightStars/BlueNebular_bottom.jpg", 
            "Resources/Skybox/NightStars/BlueNebular_front.jpg",
            "Resources/Skybox/NightStars/BlueNebular_back.jpg"
        }; 
 
        lvl->getWorld(1).setWeather(dl2, 0.02, 2, glm::vec3(0), sky1);
           
        DirectionalLight           dl(glm::vec3(0.13,0.13,0.15),glm::vec3(0.15,0.15,0.15),glm::vec3(0.3,0.3,0.35),glm::vec3(-1,-1,0));
    //    lvl->getWorld(0).setWeather(dl, 0.2, 0.5, glm::vec3(0.03,0.03,0.03), sky1);
        lvl->getWorld(0).setWeather(dl2, 0.06, 1.0, glm::vec3(0.35,0.33,0.4), sky1);
        InputHandler& ih = g->getInputHandler();
        ih.clear(); 
        loadMainGameDefaultCallbacks(&ih);
        loadMainGameMenuModeCallbacks(&ih);
        loadMainGameReadTextCallbacks(&ih);
        ih.setHandlerMode(KCALL_DEFAULT);
        
        return lvl;  
    }; 
    registerGameLevelCreate("main", GameLevelCreate(makeMain));
    registerGameLevelCreate("mainmenu", GameLevelCreate(makeMainMenu));
}

  
 
