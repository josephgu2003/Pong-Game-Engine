//
//  NotificationSystem.cpp
//  Pong
//
//  Created by Joseph Gu on 1/11/22.
//

#include "NotificationSystem.hpp"
#include "Inventory.hpp"
#include "World.hpp"
#include "InventoryComponent.hpp"

void NotificationSystem::newNotification(const std::string& text, float duration) {
    notificationQueue.push(NotificationBlurb(text, duration));
}
  
void NotificationSystem::drawAll(Renderer* r) {
    notificationFrame->draw(r);
    
}
 
void NotificationSystem::tick() { 
    if (!foundInventory) {
        if (auto x = static_cast<World*>(actor)->getPlayerHero()) {
            if (!(x->dummy)) {
                x->getComponent<InventoryComponent>()->addObserver(shared_from_this());
                foundInventory = true;
            }
        }
    }
    
    if (!notificationQueue.empty() && notificationFrame->isHidden()) {
        auto blurb = notificationQueue.front();
        notificationQueue.pop(); 
        
        if (auto x = notificationText.lock()) {
            x->setText(blurb.message);
        }
        notificationFrame->initFadeFunction(0.0, blurb.duration - 0.75f, 0.75f);
    }
}
 
NotificationSystem::NotificationSystem(World& w) : WorldSubSystem(w) {
    std::shared_ptr<uiText> notif = std::make_shared<uiText>("", -0.5, 0.8, DEFAULT_FONTSIZE, DEFAULT_LINESPACE, 1.0);
    notificationText = notif;// lmfao???
    notificationFrame = std::make_unique<uiFrame>(glm::vec2(-0.7, 0.7), glm::vec2(1.5,0.23), "Resources/GlyphsAndUI/diamondborderslim.png");
    notificationFrame->insertChild(notif);
    notificationFrame->setHiddenStatus(true);
    foundInventory = false;
}
  
 
void NotificationSystem::notify(const Subject& subject, GameEvent ge) {
    if (ge == ACQUIRE_NEW_ITEM) {
        int recentitem = static_cast<const InventoryComponent&>(subject).getLastInsertedItem();
        std::string name = ItemNames[recentitem];
        newNotification("Received " + name, 5.5f);
    }
}
