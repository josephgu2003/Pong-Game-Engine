//
//  WorldRenderingManager.cpp
//  Pong
//
//  Created by Joseph Gu on 12/12/21. 
//

#include "GraphicalScene.hpp" 
 
bool GraphicalScene::isInstancedObjHere(GraphicsObject* gc) {
    if (gc->isInstanced()) {  
        for (auto instancedobj : instancedObjects) { 
            if (auto x = instancedobj.lock()) {
                if (x.get() == gc) { 
                    return true; 
                } 
            }
        }
    }
    return false; 
}

void GraphicalScene::insertLightComponent(const std::weak_ptr<LightComponent>& gc) {
    lights.emplace_back(gc);
}
 
void GraphicalScene::insertGraphicsComponent(const std::weak_ptr<GraphicsObject>& gc) {
    if(isInstancedObjHere(gc.lock().get())) {
        return;
    }
    switch (gc.lock()->getDrawPass()) {
        case DRAW_OPAQUE: {
            opaqueObjects.emplace_back(gc);
            break; 
        }
        case DRAW_TRANSPARENT: {
            transparentObjects.emplace_back(gc);
            break;
        }
        default: {
            opaqueObjects.emplace_back(gc);
            break;
        }
    }
}

void GraphicalScene::drawAll(Renderer *r) {
    for (auto it = lights.begin(); it != lights.end(); it++) {
        if (auto x = (*it).lock()) {
            x->updateRenderer(r);
        } else {
            lights.erase(it); 
            r->updateLight(PointLight(glm::vec3(0), glm::vec3(0), glm::vec3(0), 1.0f, 1.0f, 1.0f, glm::vec3(0)));
            it--;
        }
    } 
    auto drawPass = [&] (std::vector<std::weak_ptr<GraphicsObject>>& graphicsCollection) {
        for (auto it = graphicsCollection.begin(); it != graphicsCollection.end(); it++) {
            if (auto x = (*it).lock()) {
                x->draw(r);
            } else {
                graphicsCollection.erase(it);
                it--;
            } 
        }  
    }; 
    drawPass(opaqueObjects);
    drawPass(transparentObjects);
}
