//ScoreItemPool.cpp

#include "ScoreItemPool.h"

ScoreItemPool::ScoreItemPool(size_t initialSize) {
	m_pool.resize(initialSize);
}

void ScoreItemPool::create(const Vec2& pos, int32 value) {
	for (auto& score : m_pool) {
		if (not score.isActive()) {
			score.launch(pos,value);
			return;
		}
	}

	m_pool.emplace_back().launch(pos, value);
	Print << U"Warning: BulletPool was extended. New size: " << m_pool.size();
}


void ScoreItemPool::updateAll() {
	for (auto& score : m_pool) {
		if (score.isActive()) {
			score.update();
		}
	}
}

void ScoreItemPool::drawAll() const {
	for (const auto& score : m_pool) {
		if (score.isActive()) {
			score.draw();
		}
	}
}

void ScoreItemPool::reset() {
	for (auto& score : m_pool) {
		score.deactivate();
	}
}

Array<ScoreItem>& ScoreItemPool::getItems() {
	return m_pool;
}

