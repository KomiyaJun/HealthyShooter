//BulletPool.cpp

#include "BulletPool.h"

BulletPool::BulletPool(size_t initialSize)
{
	m_pool.resize(initialSize);
}

void BulletPool::create(const Vec2& pos, const Vec2& velocity, const ColorF& color) {

	//プールから非アクティブな弾を探す
	for (auto& bullet : m_pool) {
		if (not bullet.isActive()) {
			//見つけたら、その弾を起動して処理を整える
			bullet.launch(pos, velocity, color);
			return;
		}
	}

	//空きが無ければ、プールを拡張する
	m_pool.emplace_back().launch(pos, velocity, color);
	Print << U"Warning: BulletPool was extended. New size: " << m_pool.size();
}

void BulletPool::updateAll(Speed speedM) {
	for (auto& bullet : m_pool)
	{
		if (bullet.isActive()) {
			bullet.update(speedM);
		}
	}
}

void BulletPool::drawAll() const {
	for (const auto& bullet : m_pool) {
		if (bullet.isActive()) {
			bullet.draw();
		}
	}
}

void BulletPool::reset() {
	for (auto& bullet : m_pool) {
		bullet.deactivate();
	}
}

Array<Bullet>& BulletPool::getBullets() {
	return m_pool;
}
