//Boss.h

#pragma once
#include <Siv3D.hpp>
#include "BulletPool.h"
#include "speed.h"


enum class BossState {
	Idle,
	SpiralShot,
	Shotgun,
	Dash,
};

class Boss {
public:
	Boss(const Vec2& playerPos, BulletPool& bullets);

	void update(const Vec2& playerPos, Speed speedM);
	void draw() const;
	void onHit();
	const Circle& getShape() const;
	bool isDead() const;

	bool hasCollisionDamage() const;


private:

	void updateIdle(const Vec2& playerPos);
	void updateSpiralShot(Speed speedM);
	void updateShotgun(const Vec2& playerPos, Speed speedM);
	void updateDash(const Vec2& playerPos, Speed speedM);

	void choiceNextState();


	Circle m_shape;
	int32 m_hp;
	BossState m_state;
	Stopwatch m_stateTimer;
	BulletPool& m_bulletPool;
	int32 m_shootCount;

	bool m_isDashDirections;

	double m_spiralAngle;
	Vec2 m_dashTargetPos;
};
