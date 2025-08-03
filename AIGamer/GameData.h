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

	inline int GetPlayerLevel() {
		if (gameLevel >= 4 && gameLevel < 7) {
			return 2;
		}
		else if (gameLevel >= 7 && gameLevel < 10) {
			return 3;
		}
		else if (gameLevel >= 10) {
			return 4;
		}
		else {
			return 1;
		}
	}
}
