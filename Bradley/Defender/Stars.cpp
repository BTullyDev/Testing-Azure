#include "PlayBufferPCH.h"
#include "MainGame.h"
#include "Stars.h"

Stars::Stars(Point2f pos) : BaseClass()
{
	m_pos = pos;
	m_type = BaseClass::STAR;
	SetSpriteID("star");
	m_activeTime = Play::RandomRollRange(1, 10);
	m_deactiveTime = Play::RandomRollRange(1, 10);

	// determining starting colour --- MUST be 0 OR 255;
	int i = Play::RandomRollRange(1, 3);
	if ( i == 1)
		m_r = 255;
	else if (i == 2)
		m_g = 255;
	else if (i == 3)
		m_b = 255;
}

void Stars::Draw()
{
	if (m_pos.x > Play::GetCameraPosition().x - cameraOffset.x && m_pos.x < Play::GetCameraPosition().x + DISPLAY_WIDTH + cameraOffset.x) {

		extern GameState gState;
		if (m_currentAciveTime >= gState.totalGameTime)
		{
			if (m_r > 0 && m_b == 0)
			{
				m_r--;
				m_g++;
			}
			else if (m_g > 0 && m_r == 0)
			{
				m_g--;
				m_b++;
			}
			else if (m_b > 0 && m_g == 0)
			{
				m_b--;
				m_r++;
			}


			PlayGraphics& graphics = PlayGraphics::Instance();
			graphics.ColourSprite(m_spriteID, m_r, m_g, m_b);
			graphics.Draw(m_spriteID, m_pos - Play::GetCameraPosition(), 0);
			m_currentDeactiveTime = gState.totalGameTime + m_deactiveTime;
		}
		else if (m_currentDeactiveTime < gState.totalGameTime)
		{
			m_currentAciveTime = gState.totalGameTime + m_activeTime;
		}
	}
}

void Stars::Update()
{
	//m_pos -= gState.cameraPos;
}