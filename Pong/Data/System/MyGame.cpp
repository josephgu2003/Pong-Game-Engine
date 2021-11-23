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

MyGame::MyGame() : Game() {
        
}
     
void MyGame::load() {  
    world0.setMap("Resources/Map/landscape.png", 256, 256, glm::vec3(2.0, 0.4, 2.0));
    JsonManager::loadGame(this, &actorFactory, &propFactory, &particleFactory, &scriptFactory);
      
    std::shared_ptr<HealthMeter> hm = std::make_shared<HealthMeter>();
    pHero0->getComponent<LifeComponent>()->addObserver(hm); 
    hm->notify(*pHero0->getComponent<LifeComponent>(), SUBJ_HP_CHANGED);
    ui->insertNode(hm);     
              
    DirectionalLight            dl2(glm::vec3(0.2,0.2,0.2),glm::vec3(0.8,0.8,0.8),glm::vec3(1.0,1.0,1.0),glm::vec3(-1,-1,0));
    std::vector<std::string> sky1;
    sky1.push_back("Resources/Skybox/NightStars/BlueNebular_left.jpg");
    sky1.push_back("Resources/Skybox/NightStars/BlueNebular_right.jpg");
    sky1.push_back("Resources/Skybox/NightStars/BlueNebular_top.jpg");
    sky1.push_back("Resources/Skybox/NightStars/BlueNebular_bottom.jpg");
    sky1.push_back("Resources/Skybox/NightStars/BlueNebular_front.jpg");
    sky1.push_back("Resources/Skybox/NightStars/BlueNebular_back.jpg"); // something about loading orientation of textures changes the order these files should be in
    
    world1.setWeather(dl2, 0.02, 2, glm::vec3(0), sky1);
               
    DirectionalLight         dl(glm::vec3(0.05,0.05,0.05),glm::vec3(0.15,0.15,0.15),glm::vec3(0.5,0.5,0.5),glm::vec3(-1,-1,0));
    world0.setWeather(dl, 0.02, 2, glm::vec3(0), sky1); 
                
    stbi_set_flip_vertically_on_load(1);
      
    if (pHero0.get()) {
        camera->setActor(pHero0.get());
    }
}  
  
  
 
