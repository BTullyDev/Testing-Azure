#include "PlayBufferPCH.h"
#include "MainGame.h"
#include "Lander.h"
#include "EnemyPool.h"
#include "Laser.h"
#include "Player.h"
#include "EnemyBullet.h"

Lander::Lander(Point2f pos) : EnemyBaseClass()
{
	m_pos = pos;
	SetSpriteID("lander_3");
	m_type = BaseClass::LANDER;
	m_collisionRadius = 50;
	m_velocity = { -10,0 };
	this->SetReloadTime((float)Play::RandomRoll(20));
	m_active = true;
	EnemyPool::AllocateEnemy(static_cast<EnemyBaseClass*>(this));
}

void Lander::Draw()
{
	//extern GameState gState;
	if (m_pos.x > Play::GetCameraPosition().x - cameraOffset.x && m_pos.x < Play::GetCameraPosition().x + DISPLAY_WIDTH + cameraOffset.x) {

		PlayGraphics& graphics = PlayGraphics::Instance();
		graphics.Draw(m_spriteID, m_pos - Play::GetCameraPosition(), 0);
	}
}

void Lander::Update()
{
	extern GameState gState;

	//movement
	m_pos += m_velocity;
	if (m_pos.x <= -LEVEL_WIDTH)
		m_pos.x += LEVEL_WIDTH * 2;
	else if (m_pos.x >= LEVEL_WIDTH)
		m_pos.x -= -LEVEL_WIDTH * 2;

	if (m_pos.y <= gState.UIHeight)
		m_pos.y = gState.UIHeight;
	else if (m_pos.y >= DISPLAY_HEIGHT)
		m_pos.y = DISPLAY_HEIGHT;

	//collision with lasers
	std::vector<BaseClass*> lasers = BaseClass::GetAllObjectsByType(BaseClass::LASER);

	for (int i = 0; i < lasers.size(); i++)
		if (HasCollision(*this, *lasers[i]))
		{
			m_active = false;
			//EnemyPool::DeallocateEnemy(lander);
			Laser* las = static_cast<Laser*>(lasers[i]);
			las->DestroyLaser();
		}

	if (m_pos.x > Play::GetCameraPosition().x - cameraOffset.x && m_pos.x < Play::GetCameraPosition().x + DISPLAY_WIDTH + cameraOffset.x)
	{
		std::vector<BaseClass*> players = BaseClass::GetAllObjectsByType(BaseClass::PLAYER);
		Player* player = static_cast<Player*>(players[0]);

		// vector based movement
		//m_velocity = normalize(player->GetPosition() - m_pos) * m_speed;

		// movement using dot product
		Point2f playerPos = player->GetPosition();
		Vector2f testVector = playerPos - m_pos;

		Vector2f lineVector = Vector2f{ m_pos.x, m_pos.y + 10 } - m_pos;
		Vector2f perpenLine = { lineVector.y, -lineVector.x };

		float dotProduct = dot(testVector, perpenLine);
		float d = dot(testVector, normalize(perpenLine));

		if (dotProduct < 0)
			m_velocity.x = -m_speed;
		else if (dotProduct > 0)
			m_velocity.x = m_speed;

		lineVector = Vector2f{ m_pos.x + 10, m_pos.y } - m_pos;
		perpenLine = { lineVector.y, -lineVector.x };

		dotProduct = dot(testVector, perpenLine);
		if (Play::RandomRoll(1000) <= 200)
		{
			if (dotProduct < 0)
				m_velocity.y = m_speed;
			else if (dotProduct > 0)
				m_velocity.y = -m_speed;
		}
		//testVector = normalize(testVector);
		//testVector = { testVector.y, -testVector.x };

		//float d2 = dot(testVector, m_velocity);
		//if (d2 < 0)
		//	m_velocity *= -1;

		//m_velocity = (testVector - m_pos) * 0.5f;

		//shooting
		if (GetShootTimer() <= gState.totalGameTime)
		{
			if (Play::RandomRollRange(1, 100) <= GetShootChance())
			{
				Vector2f bulletVeclocity = player->GetPosition() - m_pos;
				Point2f pos = m_pos;
				EnemyBullet::CreateEnemyBullet(pos, bulletVeclocity);
				SetShootTimer(gState.totalGameTime + GetReloadTime());
			}
		}

	}
}

//void Lander::CreateLander(Point2f pos)
//{
//	EnemyBaseClass* lander = EnemyPool::GetEnemySpace();
//	lander->SetPosition(pos);
//	lander->SetSpriteID("lander_3");
//	lander->SetType(BaseClass::LANDER);
//	lander->SetCollisionRadius(50);
//	lander->SetVelocity({ -10,0 });
//	lander->SetEnemyActive(true);
//	lander->SetReloadTime((float)Play::RandomRoll(10));
//}