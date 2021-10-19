//
//  Dice.cpp
//  Pong
//
//  Created by Joseph Gu on 10/10/21.
//

#include "Dice.hpp"

unsigned int Dice::seedValue = 0;

Dice::Dice() {
    distribution = std::uniform_int_distribution<int>(0,1000);
    unsigned int x = seedValue * std::chrono::high_resolution_clock::now().time_since_epoch().count();
    generator = std::default_random_engine{
        x
    };
    seedValue += 1;
} 
 
Dice::Dice(int inclusiveLow, int inclusiveHigh) {
    distribution = std::uniform_int_distribution<int>(inclusiveLow,inclusiveHigh);
    unsigned int x = seedValue * std::chrono::high_resolution_clock::now().time_since_epoch().count();
    generator = std::default_random_engine{
        x
    };
    seedValue++;
}

void Dice::setRange(int inclusiveLow, int inclusiveHigh) {
    distribution = std::uniform_int_distribution<int>(inclusiveLow,inclusiveHigh);
}

int Dice::roll(){
    int roll  = distribution(generator);
    return roll;
}
