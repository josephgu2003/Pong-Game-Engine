//
//  Imprisonment.cpp
//  Pong
//
//  Created by Joseph Gu on 3/4/22.
//

#include "Imprisonment.hpp"
#include "MyActorFactory.hpp"
#include "World.hpp"
#include "PropFactory.hpp"
#include "NameComponent.hpp"
#include "LifeComponent.hpp"
#include "uiLayout.hpp"
#include "AnimComponent.hpp"
#include "MovementController.hpp"
#include "uiFrame.hpp"
#include "InputHandler.hpp"

std::vector<std::string> imprisonmentCrew = { 
    "Floro", "Moonbell"
};

std::vector<std::string> imprisonmentPrereqs = {"Moonlight"};

Imprisonment::Imprisonment(World* world, bool completed, uiLayout* ui_, InputHandler* ih_) : Script(world, imprisonmentCrew, 30.0f, completed, "Imprisonment", imprisonmentPrereqs) {
    wallCooldown = 0.0f;
    imprisonmentBrightness = 3.0f;
    brightnessWatch.resetTime();
    ih = ih_;
    ui = ui_;
}

void Imprisonment::act() {
    Actor* floro = getActorNamed("Floro");
    Actor* moonbell = getActorNamed("Moonbell");
    wallCooldown -= watch.getTime();
    watch.resetTime();
    
    if (imprisonmentBrightness < 10.0f) {
        imprisonmentBrightness += brightnessWatch.getTime() * 0.07f;
        if (auto ip = imprisonment.lock()) {
            ip->getComponent<GraphicsComponent>()->setUniform("brightness", imprisonmentBrightness);
        }
        brightnessWatch.resetTime();
    }
    if (auto i = imprisonment.lock()) {
        if (floro->getDistanceTo(i.get()) > 2.0f) {
            if (wallCooldown <= 0.0f) {
                floro->getComponent<AnimComponent>()->playAnim("Falling", 24, 100, 1.5f);
                floro->getComponent<MovementController>()->paralyze(3.0f);
                floro->accelerate(0.05f * (-floro->getDir()+glm::vec3(0, 1.5, 0)));
                watch.resetTime();
                wallCooldown = 1.0f;
            }
        }
    }
    
    if (step > 3) {
        floro->getComponent<LifeComponent>()->incStatValue(-0.05, STAT_LIFE);
    }
    if (auto i = imprisonment.lock()) {
        if (getActorNamed("Hue")) {
            if (auto lc = getActorNamed("Hue")->getComponent<LifeComponent>()) {
                if (lc->getStat(STAT_LIFE).value <= 0.0f) {
                    getActorNamed("Hue")->getComponent<AnimComponent>()->playAnim("Straining", 12, 49);
                    world->deleteX(i.get());
                }
            }
        }
    }
    switch (step) {
        case 0: {
            if (!isWaiting()) {
                MyActorFactory maf;
                
                auto hue = maf.makeActor(ACTOR_SCARF_CHAR);
                hue->setPos(moonbell->getPos()+10.0f*moonbell->getDir());
                hue->orientYawTo(moonbell);
                
                auto huename = std::make_shared<NameComponent>(*hue.get());
                huename->init("Hue");
                hue->addComp(huename);
                hue->getComponent<LifeComponent>()->init(100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f);
                newActor("Hue", hue);
                
                PropFactory pf;
                auto prop = pf.makeProp(PROP_IMPRISONMENT);
                prop->setPos(moonbell->getPos());
                prop->addComponent<LightComponent>(*prop.get(), *prop.get(), PointLight(glm::vec3(3.4,1.0,1.0), glm::vec3(4.5,3.0,3.0), glm::vec3(2.0,1.0,1.0), 1.0, 0.14, 0.07, glm::vec3(0)));
                world->insert<Prop>(prop);
                speak("Moonbell", "*Gasps*", 2.0f);
                imprisonment = prop;
                floro->setPos(moonbell->getPos());
            }
            
            waitFor(5.0f);
            break;
        }
            
        case 1: {
            doAndWaitFor([&] () {speak("Floro", "We are so unlucky... this looks like a Matrix Imprisonment.", 3.0f);}, 5.0f);
            break;
        }
            
        case 2: {
            if (!isWaiting()) {
                std::vector<std::string> lines = {"The perfect alignment of mana flows to form the perfect prison.","My skills at Mana Calculation are quite scary, no?"};
                std::vector<float> durations = {3.0f, 3.0f};
                
                makeSpeech("Hue", lines, durations);
            }
            
            waitFor(9.0f);
            break;
        }
            
        case 3: {
            if (!isWaiting()) {
                std::vector<std::string> lines = {"If we stay here too long, the damage inflicted will kill us.", "Oh... *coughs*", "It seems like we are fated to look out for each other. :)", "Don't fear. Do what you do best. *cough*", "Do what you do best... Only then can you take your first steps as Shorewalker.", "I'm sorry to put such a burden on you.", "But my life is in your hands now."};
                std::vector<float> durations;
                for (int i = 0; i < lines.size(); i++) {
                    durations.push_back(3.0f);
                }
                makeSpeech("Moonbell", lines, durations);
            }
            
            waitFor(3.0f);
            break;
        }
        case 4: {
            if (!isWaiting()) {
                moonbell->getComponent<AnimComponent>()->playAnim("Straining", 12, 49);
            }
            waitFor(5.0f);
            break;
        }
        case 5: {
            if (noActiveSpeech()) {
                if (!isWaiting()) {
                    moonbell->getComponent<AnimComponent>()->playAnim("Falling", 24, 100);
                    moonbell->accelerate(0.05f * (-moonbell->getDir()+glm::vec3(0, 1.5, 0)));
                }
                waitFor(2.0f);
            }
            break;
        }
        case 6: {
            auto menu = std::make_shared<uiMenu>(glm::vec2(0.0, -0.2), glm::vec2(0.5, 0.5), TEX_EMPTY);
            menu->insertChild(std::make_shared<uiText>("I've promised...", 0.1, 0.1, DEFAULT_FONTSIZE, DEFAULT_LINESPACE, 1.0));
            menu->insertChild(std::make_shared<uiText>("But I don't...", 0.1, -0.1, DEFAULT_FONTSIZE, DEFAULT_LINESPACE, 1.0));
            choiceMenu = menu;
            ui->insertNode(menu);
            ih->addObserver(menu);
            step++;
            break;
        }
            
        case 7: {
            if (auto m = choiceMenu.lock()) {
                if (m->getChosenOption() >= 0) {
                    step += m->getChosenOption() + 1;
                    break;
                }
            }
            break;
        }
            
        case 8: {
            std::vector<std::string> lines = {"I've promised to share this umbrella.", "So I won't let the rain fall on us."};
            
            std::vector<float> durations;
            for (int i = 0; i < lines.size(); i++) {
                durations.push_back(4.0f);
            }
            makeSpeech("Floro", lines, durations);
            step += 2;
            if (auto m = choiceMenu.lock()) {
                ui->removeNode(m.get());
            }
            break;
        }
        case 9: {
            std::vector<std::string> lines = {"But I don't do anything well...", "How can you leave everything to me?"};
            
            std::vector<float> durations;
            for (int i = 0; i < lines.size(); i++) {
                durations.push_back(4.0f);
            }
            makeSpeech("Floro", lines, durations);
            floro->getComponent<AnimComponent>()->playAnim("SadTalk", true);
            step += 1;
            if (auto m = choiceMenu.lock()) {
                ui->removeNode(m.get());
            }
            break;
        }
        case 10: {
            if (noActiveSpeech()) {
                waitFor(10.0f);
            }
            break;
        }
        case 11: {
            if (noActiveSpeech() && floro->getComponent<LifeComponent>()->getStat(STAT_LIFE).value <= 0.0f) {
                floro->getComponent<AnimComponent>()->playAnim("Straining", 12, 49);
                floro->getComponent<MovementController>()->paralyze(3.0f);
                step++;
            }
            break;
            
        }
            
        case 12: {
            if (!isWaiting()) {
                auto black = std::make_shared<uiFrame>(glm::vec2(-1.0, -1.0), glm::vec2(2.0,2.0), "Resources/GlyphsAndUI/blacksquare.png");
                black->initFadeFunction(0.0, 40.0, 10.0);
                ui->insertNode(black);
            }
            waitFor(11.0f);
            break;
        }
        case 13: {
            std::vector<std::string> lines = {"Haha, Floro's painting the walls again!", "Floro can't be a Shorewalker, so Teacher is making Floro paint walls!", "If Floro could use Calculations, these paintings could be a bit neater!", "Hush, my students.", "Floro paints the walls out of passion, not because of me.", "It's what Floro does best."};
            
            for (int i = 0; i < lines.size(); i++) {
                auto ut = std::make_shared<uiText>(lines.at(i), -0.65, -0.1, DEFAULT_FONTSIZE, DEFAULT_LINESPACE * 2.0, 1.0);
                ut->initPeriodicFadeFunction(0.0f + i * 4.0f, 3.0f + i * 4.0f, 0.5f, lines.size() * 9.0f);
                ui->insertNode(ut);
            }
            step++;
            break;
        }
        default:
            break;
    }
}

// I feel that when SurvivalMode is added to tthis scene
// it feels very immersive because of the empathy the player
// feels and the level of micromanagement and involvement needed
