//sound.h

#pragma once
#include <Siv3D.hpp>


extern Optional<Audio> soundShoot;
extern Optional<Audio> soundDamage;
extern Optional<Audio> soundScore;

extern Optional<Audio> titleBGM;
extern Optional<Audio> gameBGM;
extern Optional<Audio> resultBGM;

void InitSounds();
