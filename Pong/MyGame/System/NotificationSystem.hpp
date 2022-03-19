//
//  NotificationSystem.hpp
//  Pong
//
//  Created by Joseph Gu on 1/11/22.
//
 
#ifndef NotificationSystem_hpp
#define NotificationSystem_hpp

#include <stdio.h>
#include <string>
#include <memory>
#include <vector>
#include "uiFrame.hpp"
#include "uiText.hpp"
#include "WorldSubSystem.hpp"
#include "Observer.hpp"
#include <queue>

class Renderer;

class NotificationSystem : public WorldSubSystem, public Observer, public std::enable_shared_from_this<NotificationSystem> {
private:
    struct NotificationBlurb {
        std::string message;
        float duration;
        NotificationBlurb(const std::string& message_, float duration_) {
            message = message_;
            duration = duration_;
        }
    };
    std::weak_ptr<uiText> notificationText;
    std::unique_ptr<uiFrame> notificationFrame;
    bool foundInventory;
    std::queue<NotificationBlurb> notificationQueue;
public:
    NotificationSystem(World& w);
    void newNotification(const std::string& text, float duration);
    void drawAll(Renderer* r) override;
    void tick() override;
    void notify(const Subject& subject, GameEvent ge) override;
};

#endif /* NotificationSystem_hpp */
