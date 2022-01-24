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
#include "SubtitlesSystem.hpp"
#include "ScriptSystem.hpp"
#include "Inventory.hpp"
#include "AnimComponent.hpp"
#include "MySaveGame.hpp"
#include "MyLevelSerializer.hpp"
#include "ChargedSlash.hpp"
#include "NotificationSystem.hpp"

#define POEM "I was asked - \"Do you have dreams?\"", "No...", "...Yes? Lost. Searching. Searching.", "Searching with colorful moonlight always overhead,","Yet my eyes were always down, scouring that dark canvas.","Too late, gaze up at the painted moon.", "A flash of inspiration, and the coldness of regret.","Is it too late? The moon is going away soon.","A brush dipped in lost dreams refound,", "But a hand still with regretfulness.","If only I had a pond, so that by its reflection,","I would have seen the moon's beauty sooner.","A brush, a canvas, a horizon","An artist dreaming of the moon."
#define POEM2 {"Beneath the leafless tree I hold my vigil", "A thousand snowflakes in my new home settle.", "The world is never constant, ever-changing.", "I know there will be an end to waiting."}

MyGame::MyGame() : Game() {

}
 

void MyGame::load() {
    setSaveSystem(new MySaveGame(ui));
    World::registerSubSystem<SubtitlesSystem>();
    World::registerSubSystem<ScriptSystem>();
    World::registerSubSystem<NotificationSystem>();
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
            if (auto x = game->getPlayerHero()) x->getComponent<AnimComponent>()->playAnim("Walk", true);
    });
    ih->setOneTapCallback(GLFW_KEY_S, [](Game* game){
        if (auto x = game->getPlayerHero()) x->getComponent<AnimComponent>()->playAnim("Walk", true);
    });
    ih->setOneTapCallback(GLFW_KEY_A, [](Game* game){
        if (auto x = game->getPlayerHero()) x->getComponent<AnimComponent>()->playAnim("Walk", true);
    });
    ih->setOneTapCallback(GLFW_KEY_D, [](Game* game){
        if (auto x = game->getPlayerHero()) x->getComponent<AnimComponent>()->playAnim("Walk", true);
    });
    ih->setOnReleaseCallback(GLFW_KEY_W, [](Game* game){
            if (auto x = game->getPlayerHero()) x->getComponent<AnimComponent>()->playDefault();
    });
    ih->setOnReleaseCallback(GLFW_KEY_S, [](Game* game){
        if (auto x = game->getPlayerHero()) x->getComponent<AnimComponent>()->playDefault();
    });
    ih->setOnReleaseCallback(GLFW_KEY_A, [](Game* game){
        if (auto x = game->getPlayerHero()) x->getComponent<AnimComponent>()->playDefault();
    });
    ih->setOnReleaseCallback(GLFW_KEY_D, [](Game* game){
        if (auto x = game->getPlayerHero()) x->getComponent<AnimComponent>()->playDefault();
    });

    ih->setContinuousCallback(GLFW_KEY_W, [](Game* game){
        if (auto x = game->getPlayerHero()) x->posDir(0.02);
    });
    ih->setContinuousCallback(GLFW_KEY_S, [](Game* game){
        if (auto x = game->getPlayerHero()) x->posDir(-0.02);
    });
    ih->setContinuousCallback(GLFW_KEY_A, [](Game* game){ 
        if (auto x = game->getPlayerHero()) x->posRight(0.02);
    });
    ih->setContinuousCallback(GLFW_KEY_D, [](Game* game){ 
        if (auto x = game->getPlayerHero()) x->posRight(-0.5);   
    }); 
 
    ih->setOneTapCallback(GLFW_KEY_Z, [](Game* game){
            if (auto ph = game->getPlayerHero()) {
                std::shared_ptr<Ability> fish = std::make_shared<Fish>(&ph->getWorld(), ph, 18.0);
                auto comb = ph->getComponent<CombatComponent>();
                if (comb)
                comb->newAbility(fish);
            }
        });
          
    ih->setOneTapCallback(GLFW_KEY_X, [](Game* game){
        if (auto ph = game->getPlayerHero()) { 
            ph->getComponent<AnimComponent>()->playAnim("Attack", false);
            auto comb = ph->getComponent<CombatComponent>();
            if (comb) comb->newAbility<ChargedSlash>(&ph->getWorld(), ph, 1.5);
            else {
                ph->addComponent<CombatComponent>(*ph); 
                ph->getComponent<AnimComponent>()->playAnim("DrawWeapon",false);
            }
        }
        });
         
    ih->setOneTapCallback(GLFW_KEY_M, [](Game* game){
        std::vector<std::string> lines = {POEM2};
        std::vector<float> durations;
        for (int i = 0; i < 4; i++) {
            durations.push_back(4.0f);
        }
        game->getActiveLevel()->getActiveWorld()->insert<Behaviour>(std::make_shared<Speech>(game->getPlayerHero(), lines, durations));
        game->getPlayerHero()->getComponent<AnimComponent>()->playAnim("Waiting", 73, 157);
        }); 
    ih->setOneTapCallback(GLFW_KEY_G, [](Game* game){
            if (auto ph = game->getPlayerHero()) {
                std::shared_ptr<Ability> letters = std::make_shared<FallingLetters>(&ph->getWorld(), ph, 6.0);
                auto comb = ph->getComponent<CombatComponent>();
                if (comb)
                comb->newAbility(letters);
            }
        }); 
    
    ih->setOneTapCallback(GLFW_KEY_C, [](Game* game){ 
            if (auto ph = game->getPlayerHero()) {
                std::shared_ptr<Ability> letters = std::make_shared<SwordWorld>(&ph->getWorld(), ph, 6.0);
                auto comb = ph->getComponent<CombatComponent>();
                if (comb)
                comb->newAbility(letters);
            }
        });

 
        
    ih->setOneTapCallback(GLFW_KEY_Y, [](Game* game) {
            if (auto hero = game->getPlayerHero()) {
                Actor* nearest; 
                if (!hero->getWorld().getNearestActorWith<CharacterComponent>(hero, nearest)) {
                    return;
                }
                std::shared_ptr<aiDialogueAction> dialogue = std::make_shared<aiDialogueAction>(10.0f, hero, nearest);
                std::shared_ptr<DialogueMenu> u = std::make_shared<DialogueMenu>(glm::vec2(0, 0), glm::vec2(0.5,0.5), "Resources/GlyphsAndUI/squareborder.png", dialogue);
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
    GameLevel* lvl = new GameLevel(g->getRenderer(), 1, "mainmenu");
    auto ut = std::make_shared<uiText>("Press enter to begin", -0.3, -0.8,DEFAULT_FONTSIZE, DEFAULT_LINESPACE);
    auto uf = std::make_shared<uiFrame>(glm::vec2(-1,-1), glm::vec2(2,2), "Resources/GlyphsAndUI/flowerhp.png");
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
        GameLevel* lvl = new GameLevel(g->getRenderer(), 2, "main");
        auto ui = g->getUI();
        ui->clear();
          
        loadLevelSaveFile(lvl);
        MyLevelSerializer lvlmake; 
        lvlmake.loadLevelWorlds(lvl);
        lvl->getWorld(0).setMap("Resources/Map/landscape2.png", glm::vec3(0.4, 0.0002, 0.4));
             
        auto player = lvl->getActiveWorld()->getPlayerHero();
        auto playerlife = player->getComponent<LifeComponent>();
             
        std::shared_ptr<HealthMeter> hm = std::make_shared<HealthMeter>(-0.85, 0.7);
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
        
        std::weak_ptr<InventoryComponent> invref;
        player->getComponentRef(invref);
        auto inventory = std::make_shared<Inventory>(invref,glm::vec2(-1.0,-1.0), glm::vec2(2.0,2.0));
        g->getInputHandler().addObserver(inventory);
        player->getComponent<InventoryComponent>()->addObserver(inventory);
        ui->insertNode(inventory);  
           
        auto dpt = std::make_shared<DevPosTracker>(0.5,0.8, 0.5,0.5);
        player->addObserver(dpt);
   //     ui->insertNode(dpt);
                               
        DirectionalLight            dl2(glm::vec3(0.205,0.16,0.14),glm::vec3(0.46,0.39,0.38),glm::vec3(1.3,1.3,1.4),glm::vec3(-1,-1,0));
        
        World& wOne = lvl->getWorld(1);
        World& wZero = lvl->getWorld(0);
        wOne.setDirectionalLight(dl2);
        wOne.getDistanceFog().setDistanceFog(0.03, 1, glm::vec3(0.4,0.3,0.3));
        wOne.getAtmosphere().setSkybox("Resources/Skybox/NightStars/BlueNebular_left.jpg",
                                       "Resources/Skybox/NightStars/BlueNebular_right.jpg",
                                       "Resources/Skybox/NightStars/BlueNebular_top.jpg",
                                       "Resources/Skybox/NightStars/BlueNebular_bottom.jpg",
                                       "Resources/Skybox/NightStars/BlueNebular_front.jpg",
                                       "Resources/Skybox/NightStars/BlueNebular_back.jpg");
            
        wZero.setDirectionalLight(dl2); 
        wZero.getDistanceFog().setDistanceFog(0.03, 1, glm::vec3(0.4,0.3,0.3));
        wZero.getAtmosphere().setSkybox("Resources/Skybox/NightStars/BlueNebular_left.jpg",
                                       "Resources/Skybox/NightStars/BlueNebular_right.jpg",
                                       "Resources/Skybox/NightStars/BlueNebular_top.jpg",
                                       "Resources/Skybox/NightStars/BlueNebular_bottom.jpg",
                                       "Resources/Skybox/NightStars/BlueNebular_front.jpg",
                                       "Resources/Skybox/NightStars/BlueNebular_back.jpg");
        
        InputHandler& ih = g->getInputHandler();
        ih.clear(); 
        loadMainGameDefaultCallbacks(&ih); 
        loadMainGameMenuModeCallbacks(&ih); 
        loadMainGameReadTextCallbacks(&ih);
        ih.setHandlerMode(KCALL_DEFAULT);
        
        ih.setMouseCallback("Default", [&] (double xoffset, double yoffset) {
            getPlayerHero()->rotateEuler(-0.03*xoffset, UP);
            camera->rotateEuler(-0.02*yoffset, RIGHTAXIS);
        });  
        ih.setMouseHandlerMode("Default");
        return lvl; 
    };
    registerGameLevelCreate("main", GameLevelCreate(makeMain)); 
    registerGameLevelCreate("mainmenu", GameLevelCreate(makeMainMenu));
}

   
 
    
