#include "PlayBufferPCH.h"
#include "MainGame.h"
#include "EnemyBaseClass.h"
#include "enemyBullet.h"
#include "EnemyPool.h"
#include "Player.h"

EnemyBullet::EnemyBullet(Point2f pos, Vector2f velocity) : EnemyBaseClass()
{
	m_type = BaseClass::ENEMY_BULLET;
	m_velocity = velocity;
	m_active = true;
	m_collisionRadius = m_radius;
}

void EnemyBullet::Update()
{
	m_pos += m_velocity;
	if (m_pos.x < (Play::GetCameraPosition().x - (DISPLAY_WIDTH / 2)) - cameraOffset.x || m_pos.x > Play::GetCameraPosition().x + DISPLAY_WIDTH + cameraOffset.x)
		m_active = false;

	//collision with Player
	std::vector<BaseClass*> players = BaseClass::GetAllObjectsByType(BaseClass::PLAYER);

		if (HasCollision(*this, *players[0]))
		{
			m_active = false;
			//EnemyPool::DeallocateEnemy(lander);
			Player* player = static_cast<Player*>(players[0]);
			player->Damage();
		}
}

void EnemyBullet::Draw()
{
	extern GameState gState;
	if (m_pos.x > Play::GetCameraPosition().x - cameraOffset.x && m_pos.x < Play::GetCameraPosition().x + DISPLAY_WIDTH + cameraOffset.x) {
		PlayGraphics::Instance().Draw(Play::GetSpriteId("EnemyBullet"), m_pos - Play::GetCameraPosition(), 0);
	}
}

void EnemyBullet::CreateEnemyBullet(Point2f pos, Vector2f velocity)
{
	EnemyBullet* bullet = EnemyPool::GetEnemyBulletSpace();
	if (bullet != nullptr)
	{
		bullet->SetPosition(pos);
		bullet->SetVelocity(normalize(velocity) * bullet->GetSpeed());
		bullet->SetActive(true);
		bullet->SetCollisionRadius(bullet->m_radius);
	}
}