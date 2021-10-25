//
//  Subject.cpp
//  Pong
//
//  Created by Joseph Gu on 10/17/21.
//

#include "Subject.hpp"
#include "Observer.hpp"
void Subject::notifyAll(GameEvent ge) {
    for (int i = 0 ; i < observers.size(); i++) {
        if (std::shared_ptr<Observer> o = observers.at(i).lock()) {
            o->notify(*this, ge);
        }
    }

}

void Subject::addObserver(const std::shared_ptr<Observer>& obs) {
    std::weak_ptr<Observer> o = obs;
    observers.push_back(o);
}
