//bullet.cpp

#include "bullet.h"
#include "config.h"
#include "speed.h"

//コンストラクタ
Bullet::Bullet()
	: m_pos{ 0,0 }
	, m_velocity{ 0,0 }
	, m_color{ Palette::White }
	, m_isActive {false}
{
}

void Bullet::launch(const Vec2& pos, const Vec2& velocity, const ColorF& color) {
	m_pos = pos;
	m_velocity = velocity;
	m_color = color;
	m_isActive = true;
}

void Bullet::deactivate() {
	m_isActive = false;
}

bool Bullet::isActive() const {
	return m_isActive;
}

//動作処理
void Bullet::update(Speed speedM) {
	m_pos += (m_velocity * speedM.getScale()) * Scene::DeltaTime();

	if (not Config::movableArea.stretched(200).intersects(m_pos)) {
		deactivate();
	}
}

//描画
void Bullet::draw() const {
	Circle{ m_pos, Config::enemyBulletSize }.draw(m_color);
}

//posを返す
const Vec2& Bullet::getPos() const {

	return m_pos;
}
