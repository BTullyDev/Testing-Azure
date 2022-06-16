#pragma once
#include "BaseClass.h"
class Laser : public BaseClass
{
public:
	Laser(Point2f pos, MoveDirection direction);
	void Draw() override;
	void Update() override;
	const Vector2f getLaserSpeed() { return m_laserSpeed; }
	static void CreateLaser(Point2f pos, MoveDirection direction);
	static std::vector<Laser*> s_vFreeLaserList[15]; // <--- if you change the value of the list change it in the laser.cpp as well
	void DestroyLaser();
protected:
private:
	const Vector2f m_laserSpeed{ 50,0 };
	const int m_laserCollisionRadius{ 16 };
};

