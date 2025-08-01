//BulletPool.h

#pragma once
#include "bullet.h"

class BulletPool
{
public:
	//指定した数だけ弾をプールに確保する
	BulletPool(size_t initialSize = 100);

	//プールから弾を取得し、指定したパラメータで発射
	void create(const Vec2& pos, const Vec2& velocity, const ColorF& color);

	//全てのアクティブな弾を更新
	void updateAll(Speed speedM);

	//全てのアクティブな弾を描画
	void drawAll() const;

	//当たり判定などで使うために、プールの参照を返す
	Array<Bullet>& getBullets();

	//初期化
	void reset();

private:
	Array<Bullet> m_pool;
};
