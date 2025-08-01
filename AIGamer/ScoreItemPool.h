//ScoreItemPool.h

#pragma once
#include "ScoreItem.h"

class ScoreItemPool
{
public:
	ScoreItemPool(size_t initialSize = 50);

	void create(const Vec2& pos, int32 value);

	void updateAll();

	void drawAll() const;

	Array<ScoreItem>& getItems();

	void reset();

private:
	Array<ScoreItem> m_pool;
};
