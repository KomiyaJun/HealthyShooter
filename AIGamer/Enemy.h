

// Enemy.h

#pragma once
#include <Siv3D.hpp>
#include "speed.h"

class BulletPool;

enum class EnemyState
{
	Idle,
	Chase,
	Attack,
};

String EnemyStateToString(EnemyState state);

class Enemy
{
public:
	struct EnemyParam
	{
		double recognitionRange;
		double attackRange;
		double size;
		double fireRate;
		ColorF color;
		int hp;
	};

	Enemy(const Vec2& pos, const EnemyParam& params);

	void update(const Vec2& playerPos, BulletPool& bullets, Speed speedM);
	void draw() const;
	void onHit();
	const Circle& getShape() const;
	bool isDead() const;

private:
	void updateIdle(const Vec2& playerPos);
	void updateChase(const Vec2& playerPos);
	void updateAttack(const Vec2& playerPos, BulletPool& bullets);

	// --- メンバー変数 ---
	Circle m_shape;
	Vec2 m_velocity;
	EnemyState m_state;
	EnemyParam m_params;
	double m_fireTimer = 0.0;
};
