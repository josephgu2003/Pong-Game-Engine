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
    JsonManager::loadGame(this, &actorFactory);  
    
    std::shared_ptr<HealthMeter> hm = std::make_shared<HealthMeter>();
    pHero0->getComponent<LifeComponent>()->addObserver(hm);
    ui->insertNode(hm);
        
    DirectionalLight  dl2(glm::vec3(0.2,0.2,0.2),glm::vec3(0.8,0.8,0.8),glm::vec3(1.0,1.0,1.0),glm::vec3(-1,-1,0));
    world1.setWeather(dl2, 0);
         
    DirectionalLight   dl(glm::vec3(0.03,0.03,0.03),glm::vec3(0.08,0.08,0.08),glm::vec3(0.5,0.5,0.5),glm::vec3(-1,-1,0));
    world0.setWeather(dl, 0);
     
    stbi_set_flip_vertically_on_load(1);
     
    mist = new ParticleSystem();
    
    mist->init(PE_MIST, glm::vec3(0,1.0,0));
    world0.insertParticleEffect(mist);
    
    if (pHero0.get()) {
        camera->setActor(pHero0.get());
    }
}
