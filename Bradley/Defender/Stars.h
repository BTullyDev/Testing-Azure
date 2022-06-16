#pragma once
#include "BaseClass.h"
class Stars : public BaseClass
{
public:
	Stars(Point2f pos);
	void Draw() override;
	void Update() override;
protected:
private:
	float m_activeTime{ 4.0f };
	float m_currentAciveTime{ 0.0f };
	float m_deactiveTime{ 2.0f };
	float m_currentDeactiveTime{ 0.0f };
	int m_r{ 0 };
	int m_g{ 0 };
	int m_b{ 0 };
};

