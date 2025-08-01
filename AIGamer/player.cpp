//player.cpp

#include "player.h"
#include "config.h"
#include <Siv3D.hpp>
#include "speed.h"
#include "sound.h"

Player::Player(const Vec2& pos, double radius)
	: m_shape{ pos,radius }
	, m_hp{ 10 }
	, m_speed {200.0}
	, m_maxHp{ 10 }
{
}

void Player::update(Speed speedM) {
	// 方向ベクトルをまず作成
	Vec2 direction = Vec2{ KeyD.pressed() - KeyA.pressed(), KeyS.pressed() - KeyW.pressed() };

	// 移動スピード（ベース値）とゲームスピードの倍率を分けて掛ける
	double baseSpeed = 400;
	double scaledSpeed = baseSpeed * Scene::DeltaTime() * speedM.getScale();

	// ベクトルの長さをscaledSpeedに
	Vec2 velocity = direction.isZero() ? Vec2{ 0, 0 } : direction.normalized() * scaledSpeed;

	// 移動
	m_shape.moveBy(velocity);


	//移動制限
	m_shape.x = Math::Clamp(m_shape.x, Config::movableArea.x, Config::movableArea.x + Config::movableArea.w);
	m_shape.y = Math::Clamp(m_shape.y, Config::movableArea.y, Config::movableArea.y + Config::movableArea.h);
}

void Player::draw() const {
	if (isInvincible())
	{
		// 100ミリ秒ごとに表示/非表示を切り替えて点滅を表現
		if (static_cast<int32>(m_invincibilityTimer.ms()) / 100 % 2 == 0)
		{
			m_shape.draw(HSV{ 180.0, 1.0, (m_hp / 10.0) });
		}
	}
	else // 通常時は常に表示
	{
		m_shape.draw(HSV{ 180.0, 1.0, (m_hp / 10.0) });
	}

	SimpleGUI::GetFont()(m_hp).drawAt(m_shape.center, Palette::Black);
}
void Player::onHit() {
	if (isInvincible()) {
		return;
	}

	if (m_hp > 0) {
		m_hp--;
	}

	soundDamage->playOneShot();
	m_invincibilityTimer.restart();
}

bool Player::isInvincible() const
{
	// タイマーが作動中で、設定した無敵時間を経過していなければ無敵
	return (m_invincibilityTimer.isRunning() && (m_invincibilityTimer.sF() < m_invincibilityDuration));
}

const Circle Player::getShape() const{
	//playerのx.y.rを返す
	return m_shape;
}

const int32 Player::getHp() const {
	//playerのhpを返す
	return m_hp;
}

const int32 Player::getMaxHp() const{
	return m_maxHp;
}
 
void Player::shoot(const Camera2D& camera, BulletPool& bullets) const
{
	const Mat3x2 cameraMatrix = camera.getMat3x2();
	const Mat3x2 inverseMatrix = cameraMatrix.inverse();
	const Vec2 worldMousePos = inverseMatrix.transformPoint(Cursor::PosF());
	const Vec2 direction = (worldMousePos - m_shape.center).normalized();
	const double bulletSpeed = 600.0;
	soundShoot->playOneShot();

	bullets.create(m_shape.center, direction * bulletSpeed, Palette::Blue);
}

void Player::reset(const Vec2& pos) {
	m_shape.center = pos;
	m_hp = m_maxHp;
	m_invincibilityTimer.reset();
}
