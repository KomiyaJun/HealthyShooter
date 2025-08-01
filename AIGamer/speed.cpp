//speed.cpp

#include <Siv3D.hpp>
#include "speed.h"


void Speed::setScale(double scale) {
	m_scale = scale;
}

double Speed::getScale() const {
	return m_scale;
}

void Speed::update(double target, double deltaTime) {
	m_scale = Math::Lerp(m_scale, target, deltaTime);
}
