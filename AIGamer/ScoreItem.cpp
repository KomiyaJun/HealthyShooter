//ScoreItem.cpp

#include "ScoreItem.h"


ScoreItem::ScoreItem()
	: m_initialPos{ 0,0 }
	, m_drawPos{ 0,0 }
	, m_value{ 0 }
	, m_radius{ 30.0 }
	, m_isActive{ false }
{
}

void ScoreItem::launch(const Vec2& pos, int32 value) {
	m_initialPos = pos;
	m_drawPos = pos;
	m_value = value;
	m_isActive = true;
}

void ScoreItem::deactivate() {
	m_isActive = false;
}

bool ScoreItem::isActive() const {
	return m_isActive;
}

void ScoreItem::update() {
	const double verticalOffset = Sin(Scene::Time() * 4.0) * 4.0;

	m_drawPos.y = m_initialPos.y + verticalOffset;
}

void ScoreItem::draw() const {
	Shape2D::Star(m_radius, m_drawPos)
		.draw(Palette::Gold)
		.drawFrame(1.0, Palette::Orange);
}

Circle ScoreItem::getShape() const {
	return Circle{ m_drawPos,m_radius };
}

int32 ScoreItem::getValue() const {
	return m_value;
}


