//
//  Game.hpp
//  Pong
//
//  Created by Joseph Gu on 6/3/21.
//

#ifndef Game_hpp
#define Game_hpp

#include <GL/glew.h>
#define GLFW_DLL
#include "Renderer.hpp"
#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "Actor.hpp"
#include "World.hpp"
#include "InputHandler.hpp"
#include <vector>
#include "uiLayout.hpp"
#include "Audio.hpp"
#include "GameLevel.hpp"
#include "FPSControl.hpp"
#include "SaveGame.hpp"

typedef std::function<GameLevel* (Game*)> GameLevelCreate;
typedef std::map<std::string, GameLevelCreate> LevelBuilder;

class Game {
private:
    LevelBuilder levelBuilder;
    FPSControl fpsControl;
    std::unique_ptr<GameLevel> activeLevel;
    GLFWwindow* window = NULL; // Windowed
    
    void initWindow();
    void initObjects();
    void linkObjects();
    virtual void load() = 0;
protected:
    std::unique_ptr<SaveSystem> saveSystem;
    std::shared_ptr<uiLayout> ui;
    Renderer* renderer = NULL;
    InputHandler inputHandler;
    Audio audio;

    void setLevelBuilder(LevelBuilder lvlBuilder);
    void registerGameLevelCreate(std::string levelname, GameLevelCreate glc);
    void setSaveSystem(SaveSystem* s);
    void loadLevelSaveFile(GameLevel* g);
    void saveLevel(GameLevel* g);
public:
    Actor* getPlayerHero();
    bool running = false;
    Game();
    ~Game();
    void tick(); 
    void init();
    void end();
    InputHandler& getInputHandler();
    GameLevel* getActiveLevel();
    std::shared_ptr<uiLayout>& getUI();
    void loadLevel(std::string lvl);
    Renderer* getRenderer();
};

#endif /* Game_hpp */
