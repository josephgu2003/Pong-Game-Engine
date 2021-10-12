//
//  FriendAI.hpp
//  Pong
//
//  Created by Joseph Gu on 9/29/21.
//

#ifndef FriendAI_hpp
#define FriendAI_hpp

#include <stdio.h>
#include "AIComponent.hpp"

class FriendAI : public AIComponent {
private:
    void accompanyFriend();
    Actor* friendly = NULL;
public:
    FriendAI(Actor& actor, Actor& friendly);
    virtual void tick() override;
};

#endif /* FriendAI_hpp */
