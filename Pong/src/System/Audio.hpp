//
//  Audio.hpp
//  Pong
//
//  Created by Joseph Gu on 6/10/21.
//

#ifndef Audio_hpp
#define Audio_hpp

#include <stdio.h>
#include <sndfile.h>
#include <irrKlang.h>

class Audio {
    irrklang::ISoundEngine *SoundEngine;
public:
    Audio();
    void playMusic();
};

#endif /* Audio_hpp */
