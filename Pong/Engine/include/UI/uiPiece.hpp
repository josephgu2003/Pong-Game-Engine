//
//  uiPiece.hpp
//  Pong
//
//  Created by Joseph Gu on 6/21/21.
//

#ifndef uiPiece_hpp
#define uiPiece_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "GraphicsObject.hpp"

class uiPiece;
 
typedef std::function<bool(float, uiPiece*)> FadeFunction;

class uiPiece : public GraphicsObject { // maybe do position relative in constructor, store real position internally?
private:
    FadeFunction fadeFunction;
    Watch watch;
    virtual void drawUI(Renderer* r);
    bool hidden;
    bool usingFadeFunction;
    inline void invokeFadeFunction() {
        hidden = fadeFunction(watch.getTime(), this);
    } 
    bool deleteThis;
    void initFadeFunctionAbstraction(float timeToStart, float timeToFade, float fadeDuration, bool fadeThisOnly);
    
protected:
    std::vector<std::shared_ptr<uiPiece>> children;
    glm::vec2 position;
    glm::vec2 dimensions;
    inline void drawChildren(Renderer* r) {
        for (auto ui : children) {
            ui->draw(r);
        }
    }
    
    template <typename T>
    void setUniformForAll(const std::string& s, const T& t) {
        shader->use(); // set uniforms not now
        setUniform(s, t);
        for (auto ui = children.begin(); ui != children.end(); ui++) {
            (*ui)->setUniformForAll(s, t);
        }
    } 
    
public:
    void initFadeFunction(float timeToStart, float timeToFade, float fadeDuration);
    void initFadeFunctionThisOnly(float timeToStart, float timeToFade, float fadeDuration);
    void initFadeFunction(float secPerCycle);
    void initPeriodicFadeFunction(float timeToStart, float timeToFade, float fadeDuration, float period);
    
    uiPiece(glm::vec2 position, glm::vec2 dimensions, std::string vs, std::string fs);
    void draw(Renderer* r) override final;
    void insertChild(const std::shared_ptr<uiPiece>& uip);
    
    template <typename uiType, typename... Args>
    void insertChild(Args&& ... arg) {
        children.push_back(std::make_shared<uiType>(arg...));
   }
    void setHiddenStatus(bool status);
    void flipHiddenStatus();
    bool isHidden();
    bool shouldDeleteThis();
};

#endif /* uiPiece_hpp */
