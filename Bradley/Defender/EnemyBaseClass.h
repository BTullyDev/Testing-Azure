#pragma once
#include "BaseClass.h"
class EnemyBaseClass : public BaseClass
{
public:
	EnemyBaseClass();
	~EnemyBaseClass() override;

	//virtual void Draw();
	//virtual void Update();

	int GetShootChance() { return m_shootChance; }
	//void SetShootChance(int chance) { m_shootChance = chance; }
	float GetReloadTime() { return m_reloadTime; }
	void SetReloadTime(float time) { m_reloadTime = time; }
	float GetShootTimer() { return m_shootTimer; }
	void SetShootTimer(float time) { m_shootTimer = time; }
protected:
private:
	const int m_shootChance = 25; // can be between 0-100 and 100 is a 100% chance to shoot each frame at the player
// add some reload time
	float m_reloadTime{ 0.0f };
	float m_shootTimer{ 0.0f };
};

