//Enemy.cpp

#include "Enemy.h"
#include "Bullet.h"
#include "BulletPool.h"

//デバッグ用文字
String EnemyStateToString(EnemyState state)
{
	switch (state)
	{
	case EnemyState::Idle:   return U"Idle";
	case EnemyState::Chase:  return U"Chase";
	case EnemyState::Attack: return U"Attack";
	default:                 return U"Unknown";
	}
}

//コンストラクタ
Enemy::Enemy(const Vec2& pos, const EnemyParam& params)
	: m_shape{ pos, params.size }
	, m_params{ params }
	, m_velocity{ 0, 0 }
	, m_state{ EnemyState::Idle }
	, m_fireTimer{ 0.0 } // タイマーも忘れずに初期化

{
}


//Enemyを制御
void Enemy::update(const Vec2& playerPos, BulletPool& bullets, Speed speedM)
{
	// 状態に応じて、それぞれの更新処理を呼び出す
	switch (m_state)
	{
	case EnemyState::Idle:
		updateIdle(playerPos);
		break;
	case EnemyState::Chase:
		updateChase(playerPos);
		break;
	case EnemyState::Attack:
		// updateAttackには弾リストを引き渡す
		updateAttack(playerPos, bullets);
		break;
	}

	// 状態に関わらず、最終的な座標を更新
	m_shape.center += (m_velocity * speedM.getScale()) * Scene::DeltaTime();
}

//エネミーを描画
void Enemy::draw() const
{
	// 本体を赤い円で描画
	Circle{ m_shape.center, m_params.size }.draw(m_params.color);

	SimpleGUI::GetFont()(m_params.hp).drawAt(m_shape.center, Palette::Black);
}

//エネミーのx,y,rを返す
const Circle& Enemy::getShape() const {
	return m_shape;
}

//待機
void Enemy::updateIdle(const Vec2& playerPos)
{
	m_velocity = Vec2::Zero();

	// プレイヤーが認識範囲に入ったら追跡モードへ
	if (m_shape.center.distanceFrom(playerPos) < m_params.recognitionRange)
	{
		m_state = EnemyState::Chase;
	}
}

//追跡
void Enemy::updateChase(const Vec2& playerPos)
{
	m_velocity = (playerPos - m_shape.center).setLength(100.0);

	// プレイヤーが攻撃範囲に入ったら攻撃モードへ
	if (m_shape.center.distanceFrom(playerPos) < m_params.attackRange)
	{
		m_state = EnemyState::Attack;
	}
	// 認識範囲から出たら待機モードへ
	else if (m_shape.center.distanceFrom(playerPos) >= m_params.recognitionRange)
	{
		m_state = EnemyState::Idle;
	}
}

//攻撃
void Enemy::updateAttack(const Vec2& playerPos, BulletPool& bullets)
{
	m_velocity = Vec2::Zero();
	m_fireTimer += Scene::DeltaTime();

	// 0.5秒ごとに弾を発射
	if (m_fireTimer >= m_params.fireRate)
	{
		m_fireTimer = 0.0; // タイマーをリセット

		const Vec2 direction = (playerPos - m_shape.center).normalized();
		const double speed = 300.0;

		// 自身の位置から、プレイヤーの方向へ飛ぶ弾を生成し、リストに追加
		bullets.create(m_shape.center, direction * speed, Palette::Red);
	}

	// 攻撃範囲から出たら追跡モードへ
	if (m_shape.center.distanceFrom(playerPos) > m_params.attackRange)
	{
		m_state = EnemyState::Chase;
	}

}

//ダメージ処理
void Enemy::onHit() {
	if (m_params.hp > 0) {
		m_params.hp--;
		m_params.color = Palette::Black;
	}
}

bool Enemy::isDead() const {
	return (m_params.hp <= 0);
}
