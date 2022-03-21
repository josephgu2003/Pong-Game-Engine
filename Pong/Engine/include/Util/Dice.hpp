//
//  Dice.hpp
//  Pong
//
//  Created by Joseph Gu on 10/10/21.
//

#ifndef Dice_hpp
#define Dice_hpp

#include <stdio.h>
#include <random>
#include <chrono>
 
class Dice { 
private:
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;
    static unsigned int seedValue;
public:
    Dice();
    Dice(int inclusiveLow, int inclusiveHigh);
    void setRange(int inclusiveLow, int inclusiveHigh);
    int roll();
};

 


#endif /* Dice_hpp */
