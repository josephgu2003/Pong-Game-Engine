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
#include <ik_ISound.h>
#include <string>
#include <map>

class Audio {
private: 
    irrklang::ISoundEngine *SoundEngine = NULL;
    std::map<std::string,irrklang::ISound*> activeSounds;
public:
    Audio();
    void play(const std::string& file, bool looped);
    void stopPlay(const std::string& file);
};

#endif /* Audio_hpp */
