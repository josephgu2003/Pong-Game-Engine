//
//  Observer.hpp
//  Pong
//
//  Created by Joseph Gu on 10/17/21.
//

#ifndef Observer_hpp
#define Observer_hpp

#include <stdio.h>
#include "Subject.hpp"

class Observer {
public:
    virtual void notify(const Subject& subject, GameEvent ge) = 0;
   // void subscribeTo(const Subject& s);
};
#endif /* Observer_hpp */
