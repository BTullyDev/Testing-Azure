#include "PlayBufferPCH.h"
#include "MainGame.h"
#include "EnemyBaseClass.h"
#include "Mutant.h"
#include "Laser.h" // needed for all derived classes
#include "EnemyPool.h"

Mutant::Mutant(Point2f pos) : EnemyBaseClass()
{
	m_pos = pos;
	m_spriteID = Play::GetSpriteId("mutant_2");
	m_type = BaseClass::MUTANT;
	m_collisionRadius = 50;
	m_velocity = { 10,0 };
	m_active = true;
	EnemyPool::AllocateEnemy(static_cast<EnemyBaseClass*>(this));
}

Mutant::~Mutant()
{

}

void Mutant::Update()
{
	m_pos += m_velocity;
	if (m_pos.x <= -LEVEL_WIDTH)
		m_pos.x += (LEVEL_WIDTH * 2);
	else if (m_pos.x >= LEVEL_WIDTH)
		m_pos.x -= (LEVEL_WIDTH * 2);

	//collision with lasers
	std::vector<BaseClass*> lasers = BaseClass::GetAllObjectsByType(BaseClass::LASER);

	for (int i = 0; i < lasers.size(); i++)
		if (HasCollision(*this, *lasers[i]))
		{
			m_active = false;
			//EnemyPool::DeallocateEnemy(mutant);
			Laser* lazer = static_cast<Laser*>(lasers[i]);
			lazer->DestroyLaser();
		}
}

void Mutant::Draw()
{
	if (m_pos.x > Play::GetCameraPosition().x - cameraOffset.x && m_pos.x < Play::GetCameraPosition().x + DISPLAY_WIDTH + cameraOffset.x)
		PlayGraphics::Instance().Draw(m_spriteID, m_pos - Play::GetCameraPosition(), 0);
}

void Mutant::CreateMutant(Point2f pos)
{
	EnemyBaseClass* mutant = EnemyPool::GetEnemySpace();
	mutant->SetPosition(pos);
	mutant->SetSpriteID("mutant_2");
	mutant->SetType(BaseClass::MUTANT);
	mutant->SetCollisionRadius(50);
	mutant->SetVelocity({ 10,0 });
	mutant->SetActive(true);
}