#pragma once
#include "EnemyBaseClass.h"
class Swarmer : public EnemyBaseClass
{
	Swarmer(Point2f pos);
	~Swarmer();

	static void Update(EnemyBaseClass* swarmer);
	static void Draw(EnemyBaseClass* swarmer);
};

