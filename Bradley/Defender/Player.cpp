#include "PlayBufferPCH.h"
#include "MainGame.h"
#include "Player.h"
#include "ClassStates.h"
#include "Laser.h"
#include "EnemyPool.h"
#include "EnemyBaseClass.h"

Player::Player(Point2f position) : BaseClass()
{
	m_pos = position;
	m_type = BaseClass::PLAYER;
	this->SetSpriteID("ship_right_1");
	m_collisionRadius = m_spriteWidth;
}

void Player::Draw()
{
	//extern GameState gState;
	PlayGraphics& graphics = PlayGraphics::Instance();
	graphics.Draw(m_spriteID, { m_pos.x - Play::GetCameraPosition().x, m_pos.y }, 0);

	// attempting to create the fire for the jet
	if (m_flying == true)
	{
		Point2f endOfJet = { m_pos.x - (m_spriteWidth / 2), m_pos.y };
		int loopIteration{ 0 };
		for (int i = 0; i < m_flameSprites; i++)
		{
			int offsetX = i / ((m_spriteWidth / 2) - (loopIteration / m_spriteWidth));
			int offsetY = i % m_spriteHeight;
			if (i > (offsetX * 24) + (offsetX * 2) && i != (24 - 1) + (offsetX * 24))
			{
				for (int q = 0; q < m_spriteHeightPerPixel; q++)
				{
					offsetX += q / (m_spriteHeightPerPixel / 2);
					offsetY += q % m_spriteHeightPerPixel;
					if (m_direction == RIGHT)
						graphics.DrawPixel({ endOfJet.x - offsetX - Play::GetCameraPosition().x,(m_pos.y - m_spriteHeight / 2) + offsetY - Play::GetCameraPosition().y }, { Play::RandomRollRange(1, 255), Play::RandomRollRange(1, 255), Play::RandomRollRange(1, 255) });
					else if (m_direction == LEFT)
						graphics.DrawPixel({ (endOfJet.x + m_spriteWidth) + offsetX - Play::GetCameraPosition().x,(m_pos.y - m_spriteHeight / 2) + offsetY - Play::GetCameraPosition().y }, { Play::RandomRollRange(1, 255), Play::RandomRollRange(1, 255), Play::RandomRollRange(1, 255) });
				}
			}
			loopIteration++;
		}
	}
}

void Player::Update()
{
	extern GameState gState;
	if (PlayInput::Instance().KeyDown(VK_RIGHT))
	{
		m_velocity.x += m_verticleAccelerationSpeed;
		this->SetSpriteID("ship_right_1");
		m_flying = true;
		m_direction = RIGHT;
	}
	else if (PlayInput::Instance().KeyDown(VK_LEFT))
	{
		m_velocity.x -= m_verticleAccelerationSpeed;
		this->SetSpriteID("ship_left_1");
		m_flying = true;
		m_direction = LEFT;
	}
	else
		m_flying = false;

	if (PlayInput::Instance().KeyDown(VK_UP))
	{
		m_velocity.y -= m_horizontalAccelerationSpeed;
	}
	if (PlayInput::Instance().KeyDown(VK_DOWN))
	{
		m_velocity.y += m_horizontalAccelerationSpeed;
	}

	//bomb
	if (PlayInput::Instance().KeyPressed('Z'))	// Bomb kills all enemies you can see
	{
		extern GameState gState;
		if (gState.bombs > 0)
		{
			gState.bombs--;
			for (EnemyBaseClass* p : EnemyPool::s_AllEnemies)
				if (p != nullptr && p->GetActive() == true)
					if (p->GetPosition().x > Play::GetCameraPosition().x - cameraOffset.x && p->GetPosition().x < Play::GetCameraPosition().x + DISPLAY_WIDTH + cameraOffset.x)
						p->SetActive(false);
		}
	}

	//shooting
	if (PlayInput::Instance().KeyPressed('X'))	// space does not work when holding down left + up arrow keys but all other VK_ buttons do
	{
		if (m_direction == RIGHT)
			Laser::CreateLaser({ m_pos.x, m_pos.y + m_spriteHeight / 4 }, RIGHT);
		if (m_direction == LEFT)
			Laser::CreateLaser({ m_pos.x, m_pos.y + m_spriteHeight / 4 }, LEFT);
	}

	if (m_velocity.x > m_maxHorizontalSpeed)
		m_velocity.x = m_maxHorizontalSpeed;
	if (m_velocity.x < -m_maxHorizontalSpeed)
		m_velocity.x = -m_maxHorizontalSpeed;
	if (m_velocity.y > m_maxVerticleSpeed)
		m_velocity.y = m_maxVerticleSpeed;
	if (m_velocity.y < -m_maxVerticleSpeed)
		m_velocity.y = -m_maxVerticleSpeed;

	m_velocity *= 0.94;
	m_pos += m_velocity;

	if (m_pos.y - (m_spriteHeight / 2) < 0)
	{
		m_pos.y = 0 + m_spriteHeight / 2;
	}
	if (m_pos.y + (m_spriteHeight / 2) > DISPLAY_HEIGHT)
	{
		m_pos.y = DISPLAY_HEIGHT - (m_spriteHeight / 2) + Play::GetCameraPosition().y;
	}

	if (m_pos.x > LEVEL_WIDTH)
	{
		m_pos.x -= LEVEL_WIDTH * 2;
		Play::SetCameraPosition(Play::GetCameraPosition() - Vector2f{ LEVEL_WIDTH * 2, 0 });
	}
	else if (m_pos.x < -LEVEL_WIDTH )
	{
		m_pos.x += LEVEL_WIDTH * 2;
		Play::SetCameraPosition(Play::GetCameraPosition() + Vector2f{ LEVEL_WIDTH * 2, 0 });
	}
	if (m_pos.y <= gState.UIHeight)
		m_pos.y = gState.UIHeight;
	else if (m_pos.y >= DISPLAY_HEIGHT)
		m_pos.y = DISPLAY_HEIGHT;
}

BaseClass* ReturnPlayer()
{
	for (int i = 0; i < BaseClass::s_vBaseClassObjectList.size(); i++)
	{
		if (BaseClass::s_vBaseClassObjectList[i]->GetType() == BaseClass::PLAYER)
			return BaseClass::s_vBaseClassObjectList[i];
	}
}

void Player::Damage()
{
	extern GameState gState;
	gState.lives -= 1;
	if (gState.lives < 0)
		int i = 2; // replace will a game over screen
	else if (gState.lives >= 0)
	{
		// add in some particle effects
		m_pos = { LEVEL_WIDTH / 2, DISPLAY_HEIGHT / 2 };
	}
}