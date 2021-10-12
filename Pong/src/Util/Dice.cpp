//
//  Dice.cpp
//  Pong
//
//  Created by Joseph Gu on 10/10/21.
//

#include "Dice.hpp"


Dice::Dice() {
    distribution = std::uniform_int_distribution<int>(0,1000);
}

Dice::Dice(int inclusiveLow, int inclusiveHigh) {
    distribution = std::uniform_int_distribution<int>(inclusiveLow,inclusiveHigh);
}

void Dice::setRange(int inclusiveLow, int inclusiveHigh) {
    distribution = std::uniform_int_distribution<int>(inclusiveLow,inclusiveHigh);
}

int Dice::roll(){
    int roll  = distribution(generator);
    return roll;
}
