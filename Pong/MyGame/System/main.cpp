
//  main.cpp
//  Pong
//
//  Created by Joseph Gu on 4/24/21.
//


#include <iostream>
#include "MyGame.hpp"


class Test {
public:
    long bigData[500];
    Test() {
        
    }
};

int main() {
    std::shared_ptr<Test> lotsData[200][200];
    
    int counter = 1;
     
    while (counter > 0) {
        for (int i = 0; i < 200; i++) {
            for(int j = 0; j < 200; j++) {
                lotsData[i][j] = std::make_shared<Test>();
            }
        }
        counter--;
    }
    
    for (int i = 0; i < 200; i++) {
        for(int j = 0; j < 200; j++) {
            lotsData[i][j].reset();
        }
    }
    
    MyGame game;
    
    game.init();
    while(game.running)
        
    {
        game.tick();
    }
    
    game.end();
    
    return 0;
    
}


/**
 don't copy large amounts of data
 use same vertices even when not instancing
 
 notes on performance:
 5/24: 7 % w/o, 8 % with particles of minecraft (200), no instancing
 6/3 : 18%, 713 mb 2000 particles
 6/12 :20%, 1.6 gb
 **/
