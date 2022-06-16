#include "PlayBufferPCH.h"
#include "MainGame.h"
#include "MiniScreen.h"
#include "EnemyBaseClass.h"
#include "Player.h"
#include "EnemyPool.h"

MiniScreen::MiniScreen()
{
	PlayGraphics::Instance().AddSprite(m_mapSpriteName, MiniBuffer);
	PlayGraphics::Instance().AddSprite(m_screenSpriteName, MiniBuffer);
	CreateSprite();
}
MiniScreen::~MiniScreen()
{
}

// the mini screen is {DISPLAY_WIDTH * MINI_DISPLAY_SCALE, DISPLAY_HEIGHT * MINI_DISPLAY_SCALE }
// The level is 2 * LEVEL_WIDTH
// The Level scale is the total level width / the mini screen width

void MiniScreen::Update()
{
	extern GameState gState;
	PlayGraphics& graphics = PlayGraphics::Instance();
	graphics.SetRenderTarget(&MiniBuffer);
	graphics.ClearBuffer({ 00,00,00 });

	//background drawing
	graphics.DrawRotated(m_mapSpriteID, (Vector2f{ -LEVEL_WIDTH, 0 } - Play::GetCameraPosition()) - m_pos, 0, 0, 1);
	graphics.DrawRotated(m_mapSpriteID, (Vector2f{ -LEVEL_WIDTH / 2, 0 } - Play::GetCameraPosition()) - m_pos, 0, 0, 1);
	graphics.DrawRotated(m_mapSpriteID, (Vector2f{ 0, 0 } - Play::GetCameraPosition()) - m_pos, 0, 0, 1);
	graphics.DrawRotated(m_mapSpriteID, (Vector2f{ LEVEL_WIDTH / 2, 0 } - Play::GetCameraPosition()) - m_pos, 0, 0, 1);
	graphics.DrawRotated(m_mapSpriteID, (Vector2f{ LEVEL_WIDTH, 0 } - Play::GetCameraPosition()) - m_pos, 0, 0, 1);

	// switch statement for all types within the object pool + the player
	for (EnemyBaseClass* object : EnemyPool::s_AllEnemies)
	{
		if (object != nullptr && object->GetActive() == true)
		{
			graphics.DrawRotated(object->GetSpriteID(), (Vector2f{ object->GetPosition().x, object->GetPosition().y / (LEVEL_HEIGHT / MiniBuffer.height) } - Play::GetCameraPosition() - m_pos), 0, 0, MINI_DISPLAY_SCALE);
		}
	}

	std::vector<BaseClass*> player = BaseClass::GetAllObjectsByType(BaseClass::PLAYER);
	Player* p = static_cast<Player*>(player[0]);

	graphics.UpdateSprite(m_screenSpriteName, MiniBuffer);

	graphics.SetRenderTarget(graphics.GetDrawingBuffer());
}

void MiniScreen::Draw()
{
	PlayGraphics& graphics = PlayGraphics::Instance();
	graphics.Draw(Play::GetSpriteId(m_screenSpriteName.c_str()), m_pos, 0);
}

void MiniScreen::CreateSprite()
{
	PlayGraphics& graphics = PlayGraphics::Instance();
	extern GameState gState;

	//want the length of the sprite to be the same for scrolling but the hight to shorten to fit in the mini screen
	PixelData NewBuffer2{ LEVEL_WIDTH, MiniBuffer.height, new Pixel[LEVEL_WIDTH * MiniBuffer.height] }; // same as in the MiniScreen.h
	graphics.SetRenderTarget(&MiniBuffer);
	graphics.ClearBuffer(Pixel(0x00, 0xFF, 0xFF, 0xFF));
	for (int x = 0; x < NewBuffer2.width; x++)
	{
		int y = (NewBuffer2.height * gState.perlinNoise[x]);
		//graphics.DrawPixel({ x,y }, {0, 0, 255, 255});
		NewBuffer2.pPixels[y * NewBuffer2.width + x].r = 0;
		NewBuffer2.pPixels[y * NewBuffer2.width + x].g = 0;
		NewBuffer2.pPixels[y * NewBuffer2.width + x].b = 255;
		NewBuffer2.pPixels[y * NewBuffer2.width + x].a = 255;
	}
	graphics.UpdateSprite(m_mapSpriteName, NewBuffer2);
	m_mapSpriteID = Play::GetSpriteId(m_mapSpriteName.c_str());
}