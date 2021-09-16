//
//  Numberable.hpp
//  Pong
//
//  Created by Joseph Gu on 7/9/21.
//

#ifndef Numberable_hpp
#define Numberable_hpp

#include <stdio.h>

class Numberable {
protected:
    unsigned int ID;
public:
    void setID(unsigned int ID_);
    unsigned int getID();
};


#endif /* Numberable_hpp */
