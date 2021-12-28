//
//  ScriptWakeUp.hpp
//  Pong
//
//  Created by Joseph Gu on 11/13/21.
//

#ifndef ScriptWakeUp_hpp
#define ScriptWakeUp_hpp

#include <stdio.h>
#include "Script.hpp"
#include "ParticleFactory.hpp"
#include "Particle.hpp"
#include "MyActorFactory.hpp"
#include "uiLayout.hpp"
#include "uiFrame.hpp"
#include "uiText.hpp"
struct IntroPoem {
    int step;
    float firstTime;
    std::shared_ptr<uiFrame> uf;
    std::shared_ptr<uiText> line1;
    std::shared_ptr<uiText> line2;
    std::shared_ptr<uiText> line3;
    std::shared_ptr<uiText> line4;
    std::shared_ptr<uiText> line5;
    bool tick(uiLayout* ui, float time);
};
class ScriptWakeUp : public Script {
private:
    struct IntroPoem introPoem;
    ParticleFactory pf;
    MyActorFactory maf;
    std::shared_ptr<ParticleSystem> snow;
    std::weak_ptr<uiLayout> ui;
public:
    ScriptWakeUp(World* world, bool completed, std::weak_ptr<uiLayout> ui_);
    virtual void act() override;
};


#endif /* ScriptWakeUp_hpp */
