#include "PlayBufferPCH.h"
#include "MainGame.h"
#include "EnemyBaseClass.h"
#include "EnemyBullet.h"
#include "EnemyPool.h"
#include "Lander.h"
#include "Mutant.h"

//std::vector<OverClass*> EnemyPool::s_vAllEnemies[MAX_POOL_SIZE];
EnemyBaseClass* EnemyPool::s_AllEnemies[MAX_POOL_SIZE];
EnemyBullet* EnemyPool::s_AllEnemyBullets[MAX_POOL_SIZE];

EnemyPool::EnemyPool()
{
	for (int e = 0; e < MAX_POOL_SIZE; e++)
	{
		s_AllEnemies[e] = new EnemyBaseClass();
		s_AllEnemies[e]->SetActive(false);
	}
	for (int b = 0; b < MAX_POOL_SIZE; b++)
	{
		s_AllEnemyBullets[b] = new EnemyBullet({ 0,0 }, { 0,0 });
		s_AllEnemyBullets[b]->SetActive(false);
	}
}

//void EnemyPool::UpdateAll()
//{
//	for (EnemyBaseClass* object : s_AllEnemies)
//	{
//		if (object != nullptr && object->GetEnemyActive() == true)
//		{
//			switch (object->GetType())
//			{
//			case BaseClass::LANDER:
//				//static_cast<Lander*>(object);
//				//object->Update();
//				break;
//			case BaseClass::MUTANT:
//				Mutant::Update(object);
//				break;
//			case BaseClass::BAITER:
//				break;
//			case BaseClass::BOMBER:
//				break;
//			case BaseClass::POD:
//				break;
//			case BaseClass::SWARMER:
//				break;
//
//				break;
//			}
//		}
//	}
//
//	for (EnemyBullet* object : s_AllEnemyBullets)
//	{
//		if (object != nullptr && object->GetEnemyActive() == true)
//		{
//			EnemyBullet::Update(object);
//		}
//	}
//
//}
//
//void EnemyPool::DrawAll()
//{
//	for (EnemyBaseClass* object : s_AllEnemies)
//	{
//		if (object != nullptr && object->GetEnemyActive() == true)
//		{
//			switch (object->GetType())
//			{
//			case BaseClass::LANDER:
//				//object = static_cast<Lander*>(object);
//				//object->Draw();
//				break;
//			case BaseClass::MUTANT:
//				Mutant::Draw(object);
//				break;
//			case BaseClass::BAITER:
//				break;
//			case BaseClass::BOMBER:
//				break;
//			case BaseClass::POD:
//				break;
//			case BaseClass::SWARMER:
//				break;
//
//				break;
//			}
//		}
//	}
//
//	for (EnemyBullet* object : s_AllEnemyBullets)
//	{
//		if (object != nullptr && object->GetEnemyActive() == true)
//		{
//			EnemyBullet::Draw(object);
//		}
//	}
//}

void EnemyPool::AllocateEnemy(EnemyBaseClass* oc)
{
	for (int e = 0; e < MAX_POOL_SIZE; e++)
		if (s_AllEnemies[e] == nullptr || s_AllEnemies[e]->GetActive() == false)
		{
			s_AllEnemies[e] = oc;
			break;
		}
}
EnemyBaseClass* EnemyPool::GetEnemySpace()
{
	for (int e = 0; e < MAX_POOL_SIZE; e++)
		if (s_AllEnemies[e] == nullptr || s_AllEnemies[e]->GetActive() == false)
		{
			return s_AllEnemies[e];
			break;
		}
}
void EnemyPool::AllocateEnemyBullet(EnemyBullet* oc)
{
	for (int b = 0; b < MAX_POOL_SIZE; b++)
		if (s_AllEnemyBullets[b] == nullptr || s_AllEnemyBullets[b]->GetActive() == false)
		{
			s_AllEnemyBullets[b] = oc;
			break;
		}
}
EnemyBullet* EnemyPool::GetEnemyBulletSpace()
{
	for (int b = 0; b < MAX_POOL_SIZE; b++)
		if (s_AllEnemyBullets[b] == nullptr || s_AllEnemyBullets[b]->GetActive() == false)
		{
			return s_AllEnemyBullets[b];
			break;
		}
	return nullptr;
}

//void EnemyPool::DeallocateEnemy(OverClass* oc)
//{
//	for (int e = 0; e < MAX_POOL_SIZE; e++)
//		if (s_AllEnemies[e] == oc)
//		{
//			delete s_AllEnemies[e];
//			break;
//		}
//}