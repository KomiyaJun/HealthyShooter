//config.h

#pragma once
#include <Siv3D.hpp>

namespace Config
{
	//ゲームのステージ(移動可能な範囲)
	constexpr RectF movableArea{ -1000,-1000,2000,2000 };
	//敵弾のサイズ
	constexpr float enemyBulletSize = 10;



	//===============
	//エネミー管理	 
	// ==============
	
	//Wave1の出現数
	constexpr int wave1EnemyNum = 5;
	constexpr int wave1SpawnThreshold = 1;
	//Wave2の出現数
	constexpr int wave2EnemyNum = 8;
	constexpr int wave2SpawnThreshold = 3;
	//Wave3の出現数
	constexpr int wave3EnemyNum = 10;
	constexpr int wave3SpawnThreshold = 3;
}
