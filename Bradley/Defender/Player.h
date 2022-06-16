#pragma once
#include "BaseClass.h"
class Player : public BaseClass
{
public:
	Player(Point2f position);
	void Draw() override;
	void Update() override;
	static Player* ReturnPlayer();

	MoveDirection GetDirection() { return m_direction; }
	void SetDirection(MoveDirection direction) { m_direction = direction; }

	void Damage();
protected:
	bool m_flying{ false };
private:
	const int m_verticleAccelerationSpeed{ 2 };
	const int m_horizontalAccelerationSpeed{ 1 };
	const int m_maxVerticleSpeed{ 50 };
	const int m_maxHorizontalSpeed{ 20 };
	const int m_spriteWidth{ 60 };
	const int m_spriteHeight{ 24 };
	const int m_spriteHeightPerPixel{ m_spriteHeight / 6 };
	const int m_flameSprites{ 240 };
};

