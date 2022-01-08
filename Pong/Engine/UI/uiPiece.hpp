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

typedef std::function<void(float, Shader*)> FadeFunction;

class uiPiece : public GraphicsObject { // maybe do position relative in constructor, store real position internally?
private:
    FadeFunction fadeFunction;
    Watch watch;
protected:
    std::vector<std::shared_ptr<uiPiece>> children;
    glm::vec2 position;
    glm::vec2 dimensions;
    inline void drawChildren(Renderer* r) {
        for (auto ui : children) {
            ui->draw(r);
        } 
    }
    
    void initFadeFunction(float timeToStart, float timeToFade, float fadeDuration);
    inline void invokeFadeFunction() { 
        fadeFunction(watch.getTime(), shader);
    }
    template <typename T>
    void setUniformForAll(const std::string& s, const T& t) {
        shader->use(); // set uniforms not now
        setUniform(s, t);
        for (auto ui : children) {
            ui->setUniformForAll(s, t);
        }
    }
    
public:
    uiPiece(glm::vec2 position, glm::vec2 dimensions, std::string vs, std::string fs);
    virtual void draw(Renderer* r) override;
    void insertChild(const std::shared_ptr<uiPiece>& uip);
    
    template <typename uiType, typename... Args>
    void insertChild(Args&& ... arg) {
        children.push_back(std::make_shared<uiType>(arg...));
   }
};

#endif /* uiPiece_hpp */
