//
//  ScriptEvent.hpp
//  Pong
//
//  Created by Joseph Gu on 7/2/21.
//

#ifndef Script_hpp
#define Script_hpp

#include <stdio.h>
#include <vector>
#include <unordered_map>
#include "Positionable.hpp"
#include "Actor.hpp"
#include "Watch.hpp"
#include "Speech.hpp"
#include "Camera.hpp"

class World; 

class Script : public Positionable {
private:
    bool prerequisitesDone;
    std::vector<std::string> prerequisiteScenes;
    std::string sceneName;
    bool checkAllHere();
    std::shared_ptr<Actor> dummy;
    float lastTime;
    float radius;
    std::unordered_map<std::string, std::weak_ptr<Actor>> actors;
    bool completed;
    std::weak_ptr<Speech> speechRef;
    void checkPrerequisites();
protected:
    std::weak_ptr<Camera> camRef;
    Watch stopWatch;
    int step;
    World* world = nullptr;
    Actor* getActorNamed(std::string name);
    std::weak_ptr<Actor> getActorRefNamed(std::string name);
    void incStep(bool resetTime);
    void waitFor(float duration);
    bool isWaiting();
    void newActor(std::string name, const std::shared_ptr<Actor>& actor);
    void endScene();
    void speak(const std::string& speaker, const std::string& spoken, float duration);
    void makeSpeech(std::string speaker, std::vector<std::string>& lines, std::vector<float>& durations);
    bool noActiveSpeech();
    void lockCamToPlayer();
    void focusCamOnActor(glm::vec3 offset, const std::string& actor);
public:
    Script(World* world, std::vector<std::string> crew, float radius, bool completed, std::string scenenName, std::vector<std::string> prerequisiteScenes_);
    void tick();
    virtual void act() = 0;
    bool isComplete();
    const std::string& getName();
};

#endif /* ScriptEvent_hpp */
