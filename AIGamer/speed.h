//speed.h

#pragma once

class Speed {
public:
	void setScale(double scale);
	double getScale() const;
	void update(double target, double deltaTime);


private:
	double m_scale = 1.0;
};
