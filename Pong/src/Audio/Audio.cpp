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

void Audio::playMusic() {
SoundEngine->play2D("Resources/Audio/Music/New Recording 56.wav", true);
}

