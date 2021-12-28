//
//  Audio.cpp
//  Pong
//
//  Created by Joseph Gu on 6/10/21.
//

#include "Audio.hpp"

Audio::Audio() {
    SoundEngine = irrklang::createIrrKlangDevice();
}

void Audio::play(const std::string& file, bool looped) {
    SoundEngine->play2D(file.c_str(), looped);
}

