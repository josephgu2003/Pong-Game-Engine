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

MyGame::MyGame() : Game() {
        
}
     
void MyGame::load() {
    JsonManager::loadGame(this, &actorFactory, &propFactory, &particleFactory, &scriptFactory);
      
    std::shared_ptr<HealthMeter> hm = std::make_shared<HealthMeter>();
    pHero0->getComponent<LifeComponent>()->addObserver(hm); 
    hm->notify(*pHero0->getComponent<LifeComponent>(), SUBJ_HP_CHANGED);
    
    std::shared_ptr<uiFrame> uf = std::make_shared<uiFrame>(glm::vec2(-0.9,0.65), glm::vec2(0.3,0.3), TEX_BLACK_GRADIENT);
    uf->insertChild(hm);
    ui->insertNode(uf);      
              
    DirectionalLight            dl2(glm::vec3(0.2,0.2,0.2),glm::vec3(0.8,0.8,0.8),glm::vec3(1.0,1.0,1.0),glm::vec3(-1,-1,0));
    std::vector<std::string> sky1;
    sky1.push_back("Resources/Skybox/NightStars/BlueNebular_left.jpg");
    sky1.push_back("Resources/Skybox/NightStars/BlueNebular_right.jpg");
    sky1.push_back("Resources/Skybox/NightStars/BlueNebular_bottom.jpg");
    sky1.push_back("Resources/Skybox/NightStars/BlueNebular_top.jpg");
    sky1.push_back("Resources/Skybox/NightStars/BlueNebular_front.jpg");
    sky1.push_back("Resources/Skybox/NightStars/BlueNebular_back.jpg");
// something about loading orientation of textures changes the order these files should be in
    
    world1.setWeather(dl2, 0.02, 2, glm::vec3(0), sky1);
                 
    DirectionalLight          dl(glm::vec3(0.13,0.13,0.133),glm::vec3(0.15,0.15,0.155),glm::vec3(0.3,0.3,0.3),glm::vec3(-1,-1,0));
    world0.setWeather(dl, 0.005, 3, glm::vec3(0.0,0.0,0.0), sky1);
                       
    stbi_set_flip_vertically_on_load(1);  
         
    if (pHero0.get()) {  
        camera->setActor(pHero0.get());
        world0.setPlayerHero(pHero0);
    }  
    world0.setMap("Resources/Map/test.png", 1024, 1024, glm::vec3(0.5, 0.1, 0.5)); 
}   
 
     
 
  
