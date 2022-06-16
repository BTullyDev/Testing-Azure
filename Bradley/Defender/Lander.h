#pragma once
#include "EnemyBaseClass.h"
class Lander : public EnemyBaseClass
{
public:
	Lander(Point2f pos);
	void Draw() override;
	void Update() override;

	//static void CreateLander(Point2f pos);

protected:
	float m_speed = 10;
private:
};

