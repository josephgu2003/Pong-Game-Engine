//
//  Subject.hpp
//  Pong
//
//  Created by Joseph Gu on 10/17/21.
//

#ifndef Subject_hpp
#define Subject_hpp

#include <stdio.h>
#include <vector>
#include <memory>

enum GameEvent {
    SUBJ_HP_CHANGED, KEY_PRESSED, SUBJ_MANA_CHANGED, SUBJ_POS_CHANGED
};

class Observer;

class Subject {
private:
    std::vector<std::weak_ptr<Observer>> observers;  
public:
    void notifyAll(GameEvent ge);
    void addObserver(const std::shared_ptr<Observer>& obs); 
};
#endif /* Subject_hpp */
