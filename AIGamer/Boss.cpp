//Boss.cpp

#include "Boss.h"
#include "GameData.h"

Boss::Boss(const Vec2& pos, BulletPool& bullets)
	: m_shape{ pos,200 }
	, m_hp{ 1 }
	, m_state{ BossState::Idle }
	, m_bulletPool{ bullets }
	, m_shootCount{0}
	, m_isDashDirections{false}
	, m_spiralAngle{0.0}
	, m_dashTargetPos{Vec2::Zero()}
{
	m_stateTimer.start();
}

void Boss::update(const Vec2& playerPos, Speed speedM) {
	switch (m_state)
	{
	case BossState::Idle:
		updateIdle(playerPos);
		break;
	case BossState::SpiralShot:
		updateSpiralShot(speedM);
		break;
	case BossState::Shotgun:
		updateShotgun(playerPos, speedM);
		break;
	case BossState::Dash:
		updateDash(playerPos, speedM);
		break;
	}
}
void Boss::updateIdle(const Vec2& playerPos) {
	if (m_stateTimer > 2.0s) {
		choiceNextState();
	}
}

void Boss::updateSpiralShot(Speed speedM) {
	if (m_stateTimer.ms() % (10 / GameData::gameLevel) == 0)
	{
		const Vec2 velocity = Vec2::Right().rotated(m_spiralAngle).setLength(300);
		m_bulletPool.create(m_shape.center, velocity, Palette::Magenta);
	}

	m_spiralAngle += 20.0 * speedM.getScale() * Scene::DeltaTime();

	if (m_stateTimer > 5s) {
		m_state = BossState::Idle;
		m_stateTimer.restart();
	}
}


void Boss::updateShotgun(const Vec2& playerPos, Speed speedM) {
	const int32 shootLimit = 3;

	const int shootNum = 10 * GameData::gameLevel;
	if (m_stateTimer > 1.5s) {
		const Vec2 toPlayer = (playerPos - m_shape.center).normalize();

		for (int i = -shootNum; i <= shootNum; i++) {
			Vec2 velocity = toPlayer.rotated(i * 10_deg).setLength(500);
			m_bulletPool.create(m_shape.center, velocity, Palette::Orange);
		}
		m_shootCount++;
		m_stateTimer.restart();
	}

	if (m_shootCount >= shootLimit && m_state == BossState::Shotgun) {
		m_shootCount = 0;
		m_state = BossState::Idle;
		m_stateTimer.restart();
	}
}

void Boss::updateDash(const Vec2& playerPos, Speed speedM) {
	if (not m_isDashDirections) {
		m_dashTargetPos = (playerPos - m_shape.center).normalized();
		m_isDashDirections = true;
	}

	if (m_stateTimer.sF() < 0.5) {
		//予備動作
	}
	else { // 突進開始
		const Vec2 velocity = m_dashTargetPos.setLength(1200);
		m_shape.center += velocity * speedM.getScale() * Scene::DeltaTime();
	}

	if (m_stateTimer > 1s) {
		m_state = BossState::Idle;
		m_stateTimer.restart();
	}
}

void Boss::choiceNextState() {
	const int32 next = Random(1, 3);

	if (next == 1)	m_state = BossState::SpiralShot;
	else if (next == 2) m_state = BossState::Shotgun;
	else {
		m_state = BossState::Dash;
		m_isDashDirections = false;
	}

	m_stateTimer.restart();
}

bool Boss::hasCollisionDamage() const {
	return(m_state == BossState::Dash) && (m_stateTimer.sF() > 0.5);
}

void Boss::draw() const
{
	if (m_state == BossState::Dash && m_stateTimer.sF() > 0.5) {
		m_shape.draw(Palette::Tomato);
		SimpleGUI::GetFont()(m_hp).drawAt(m_shape.center, Palette::Black);

	}
	else {
		// 本体を赤い円で描画
		Circle{ m_shape.center, m_shape.r }.draw(Palette::Red);
		SimpleGUI::GetFont()(m_hp).drawAt(m_shape.center, Palette::Black);

	}
}

//ボスのx,y,rを返す
const Circle& Boss::getShape() const {
	return m_shape;
}


//ダメージ処理
void Boss::onHit() {
	if (m_hp > 0) {
		m_hp--;
	}
}

bool Boss::isDead() const {
	return (m_hp <= 0);
}
