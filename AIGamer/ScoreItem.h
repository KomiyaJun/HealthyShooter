//ScoreItem.h

#pragma once
#include <Siv3D.hpp>

class ScoreItem
{
public:
	ScoreItem();

	void launch(const Vec2& pos, int32 value);

	void deactivate();

	bool isActive() const;

	void update();

	void draw() const;

	Circle getShape() const;

	int32 getValue() const;


private:
	Vec2 m_initialPos;
	Vec2 m_drawPos;
	int32 m_value;
	double m_radius;
	bool m_isActive;
};
