//player.h

#pragma once
#include <Siv3D.hpp>
#include "bullet.h"
#include "speed.h"
#include "BulletPool.h"

class Bullet;

class Player {
public:
	Player(const Vec2& pos, double radius);

	//プレイヤー制御
	void update(Speed speedM);
	//プレイヤー描画
	void draw() const;	
	//衝突時の機能
	void onHit();
	//位置&サイズ
	const Circle getShape() const;
	const int32 getHp() const;
	const int32 getMaxHp() const;

	bool isInvincible() const;
	//弾を発射
	void shoot(const Camera2D& camera, BulletPool& bullets) const;

	void reset(const Vec2& pos);
private:
	Circle m_shape;
	int32 m_hp;
	int32 m_maxHp;
	float m_speed;

	Stopwatch m_invincibilityTimer;
	const double m_invincibilityDuration = 1.5; // 無敵時間（秒）
};
