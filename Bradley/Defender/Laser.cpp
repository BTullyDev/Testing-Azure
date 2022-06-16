#include "PlayBufferPCH.h"
#include "MainGame.h"
#include "Laser.h"

std::vector<Laser*> Laser::s_vFreeLaserList[15]; // <--- if you change the value of the list change it in the laser.h as well

Laser::Laser(Point2f pos, MoveDirection direction) : BaseClass()
{
	m_pos = pos;
	m_type = BaseClass::LASER;
	SetSpriteID("tempLaser");
	m_collisionRadius = m_laserCollisionRadius;
	if (direction == RIGHT)
		m_velocity = m_laserSpeed;
	else if (direction == LEFT)
		m_velocity = -m_laserSpeed;
}

void Laser::Draw()
{
	PlayGraphics& graphics = PlayGraphics::Instance();
	graphics.Draw(m_spriteID, m_pos - Play::GetCameraPosition(), 0);
}

void Laser::Update()
{
	m_pos += m_velocity;

	if (m_pos.x < (Play::GetCameraPosition().x - (DISPLAY_WIDTH / 2)) - cameraOffset.x || m_pos.x > Play::GetCameraPosition().x + DISPLAY_WIDTH + cameraOffset.x)
	{
		DestroyLaser();
	}

}

void Laser::CreateLaser(Point2f pos, MoveDirection direction)
{
	if (Laser::s_vFreeLaserList->empty())
	{
		if (direction == RIGHT)
			new Laser(pos, direction);
		if (direction == LEFT)
			new Laser(pos, direction);
	}
	else
	{
		Laser* l = Laser::s_vFreeLaserList->back();
		Laser::s_vFreeLaserList->pop_back();
		l->m_pos = pos;
		l->m_collisionRadius = l->m_laserCollisionRadius;
		if (direction == RIGHT)
			l->m_velocity = l->getLaserSpeed();
		if (direction == LEFT)
			l->m_velocity = -l->getLaserSpeed();
		s_vBaseClassObjectList.push_back(l);
	}
}

void Laser::DestroyLaser()
{
	s_vFreeLaserList->push_back(this);
	s_vBaseClassObjectList.erase(find(s_vBaseClassObjectList.begin(), s_vBaseClassObjectList.end(), static_cast<BaseClass*>(this)));
	//m_active = false;
}