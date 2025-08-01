//bullet.h

#pragma once
#include <Siv3D.hpp>
#include "speed.h"

class Bullet
{
public:
	Bullet();

	//弾を初期化,「使用中」に
	void launch(const Vec2& pos, const Vec2& velocity, const ColorF& color);
	//弾を「未使用」に戻す
	void deactivate();
	//状態を返す
	bool isActive() const;


	void update(Speed speedM);
	void draw() const;
	const Vec2& getPos() const;

private:
	Vec2 m_pos;
	Vec2 m_velocity;
	ColorF m_color;
	bool m_isActive;
};
