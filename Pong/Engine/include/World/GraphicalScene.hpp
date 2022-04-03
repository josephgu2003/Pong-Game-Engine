//
//  WorldRenderingManager.hpp
//  Pong
//
//  Created by Joseph Gu on 12/12/21.
//

#ifndef WorldRenderingManager_hpp
#define WorldRenderingManager_hpp

#include <stdio.h>
#include <memory>
#include "GraphicsObject.hpp"
#include "LightComponent.hpp"

class Renderer;

class GraphicalScene {
    std::vector<std::weak_ptr<GraphicsObject>> opaqueObjects;
    std::vector<std::weak_ptr<GraphicsObject>> transparentObjects;
    std::vector<std::weak_ptr<GraphicsObject>> instancedObjects;
    std::vector<std::weak_ptr<LightComponent>> lights;
public:
    bool isInstancedObjHere(GraphicsObject* gc);
    void insertGraphicsComponent(const std::weak_ptr<GraphicsObject>& gc);
    void drawAll(Renderer* r);
    void insertLightComponent(const std::weak_ptr<LightComponent>& gc);
};

#endif /* WorldRenderingManager_hpp */
