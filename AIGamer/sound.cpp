//sound.cpp

#include "sound.h"

// グローバル変数の型を Optional<Audio> に変更
Optional<Audio> soundShoot;
Optional<Audio> soundDamage;
Optional<Audio> soundScore;


Optional<Audio> titleBGM;
Optional<Audio> gameBGM;
Optional<Audio> resultBGM;

void InitSounds()
{
	// 代入(=)から .emplace() に変更
	soundShoot.emplace(U"assets/SE/Shoot.mp3");
	soundDamage.emplace(U"assets/SE/Damage.mp3");
	soundScore.emplace(U"assets/SE/GetScore.mp3");

	titleBGM.emplace(U"assets/BGM/TitleMain.mp3",Loop::Yes);
	gameBGM.emplace(U"assets/BGM/GameMain.mp3", Loop::Yes);
	resultBGM.emplace(U"assets/BGM/ResultMain.mp3", Loop::Yes);
}
