#pragma once
class EnemyBaseClass;
class EnemyBullet;

constexpr int MAX_POOL_SIZE = 50;

class EnemyPool
{
public:
	EnemyPool();

	//static void UpdateAll();
	//static void DrawAll();

	static EnemyBaseClass* s_AllEnemies[MAX_POOL_SIZE];
	static EnemyBullet* s_AllEnemyBullets[MAX_POOL_SIZE];

	static void AllocateEnemy(EnemyBaseClass* oc);
	static EnemyBaseClass* GetEnemySpace();
	static void AllocateEnemyBullet(EnemyBullet* oc);
	static EnemyBullet* GetEnemyBulletSpace();
	//static void DeallocateEnemy(OverClass* oc);

protected:
};

