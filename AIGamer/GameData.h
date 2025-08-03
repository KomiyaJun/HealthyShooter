//GameData.h
#pragma once

namespace GameData {
	inline int gameLevel = 1;
	inline int highScore = 0;

	inline void OnGameClear() {
		gameLevel++;
	}

	inline void Reset() {
		gameLevel = 1;
	}
}
