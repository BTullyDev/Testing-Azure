#include "PlayBufferPCH.h"

#include "MainGame.h"
#include "Player.h"
#include "Stars.h"
#include "Lander.h"
#include "Mutant.h"
#include "Background.h"
#include "MiniScreen.h"
#include "EnemyPool.h"

PixelData g_miniScreen;
GameState gState;

constexpr int NUMBER_OF_STARS = 100;
constexpr int NUMBER_OF_LANDERS = 10;
constexpr int NUMBER_OF_MUTANTS = 10;

// The entry point for a PlayBuffer program
void MainGameEntry(PLAY_IGNORE_COMMAND_LINE)
{
	// The graphics object creates a drawing buffer and loads any sprites in the directory provided
	PlayGraphics& graphics = PlayGraphics::Instance(DISPLAY_WIDTH, DISPLAY_HEIGHT, "Data\\Sprites\\");
	//graphics.LoadBackground( "Data\\Backgrounds\\background.png" ); 
	graphics.CentreAllSpriteOrigins();

	// The window object manages the window for us using the drawing buffer from the graphics object
	PlayWindow::Instance(PlayGraphics::Instance().GetDrawingBuffer(), DISPLAY_SCALE);

	// The audio object manages mp3 sounds and music
	PlayAudio& audio = PlayAudio::Instance("Data\\Audio\\");
	//audio.StartAudio( "DefenderIntro", false ); 

		// We allocate our own memory for a separate mini drawing buffer
	g_miniScreen = { MINI_WIDTH, MINI_HEIGHT, new Pixel[MINI_WIDTH * MINI_HEIGHT] };
	// Once we've added this as a sprite its buffer will be freed up automatically by PlayGraphics
	//graphics.AddSprite("MINI SCREEN", g_miniScreen);

	GetBackground(LEVEL_WIDTH / 2, DISPLAY_HEIGHT / 2, "Background");
	gState.miniScreen = new MiniScreen(); // must be called after the background creation
	gState.UIHeight = gState.miniScreen->GetMiniBuffer().height;

	new EnemyPool();

	for (int s = 0; s < NUMBER_OF_STARS; s++)
		new Stars({ Play::RandomRollRange(-DISPLAY_WIDTH * 3, DISPLAY_WIDTH * 3), Play::RandomRollRange(gState.miniScreen->GetPosition().height + gState.miniScreen->GetMiniBuffer().height, DISPLAY_HEIGHT / 1.5) });

	for (int l = 0; l < NUMBER_OF_LANDERS; l++)
	{
		//Lander::CreateLander({ Play::RandomRollRange(-DISPLAY_WIDTH * 2, DISPLAY_WIDTH * 2), Play::RandomRollRange(0, DISPLAY_HEIGHT / 1.5) });
		new Lander({ Play::RandomRollRange(-DISPLAY_WIDTH * 2, DISPLAY_WIDTH * 2), Play::RandomRollRange(gState.miniScreen->GetPosition().height + gState.miniScreen->GetMiniBuffer().height, DISPLAY_HEIGHT / 1.5) }); // remove this later when I have a enemy pool
	}
	for (int l = 0; l < NUMBER_OF_MUTANTS; l++)
		new Mutant({ Play::RandomRollRange(-DISPLAY_WIDTH * 2, DISPLAY_WIDTH * 2), Play::RandomRollRange(gState.miniScreen->GetPosition().height + gState.miniScreen->GetMiniBuffer().height, DISPLAY_HEIGHT / 1.5) }); // remove this later when I have a enemy pool

	new Player({ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 });
}

// Called by the PlayBuffer every frame (60 times a second!)
bool MainGameUpdate(float elapsedTime)
{
	gState.totalGameTime += elapsedTime;

	PlayGraphics& graphics = PlayGraphics::Instance();

	static int spr_font = graphics.GetSpriteId("font36px");

	// Reset the timing bar data and use green for the background image draw
	graphics.TimingBarBegin(PIX_GREEN);
	//graphics.DrawBackground();
	graphics.ClearBuffer({ 00,00,00 });

	//std::vector<OverClass*> player = OverClass::GetAllObjectsByType(OverClass::PLAYER);
	//Player* p = static_cast<Player*>(player[0]);

	UpdateCamera();

	// make these loop when unseen and not draw while unseen to help speed things up
	graphics.Draw(gState.backgroundID, Point2f{ 0,DISPLAY_HEIGHT * 0.5f } - Play::GetCameraPosition(), 0);
	graphics.Draw(gState.backgroundID, Point2f{ LEVEL_WIDTH / 2,DISPLAY_HEIGHT * 0.5f } - Play::GetCameraPosition(), 0);
	graphics.Draw(gState.backgroundID, Point2f{ LEVEL_WIDTH * 1,DISPLAY_HEIGHT * 0.5f } - Play::GetCameraPosition(), 0);
	graphics.Draw(gState.backgroundID, Point2f{ LEVEL_WIDTH * 1.5f,DISPLAY_HEIGHT * 0.5f } - Play::GetCameraPosition(), 0);

	graphics.Draw(gState.backgroundID, Point2f{ -LEVEL_WIDTH / 2,DISPLAY_HEIGHT * 0.5f } - Play::GetCameraPosition(), 0);
	graphics.Draw(gState.backgroundID, Point2f{ -LEVEL_WIDTH ,DISPLAY_HEIGHT * 0.5f } - Play::GetCameraPosition(), 0);
	graphics.Draw(gState.backgroundID, Point2f{ -LEVEL_WIDTH * 1.5f,DISPLAY_HEIGHT * 0.5f } - Play::GetCameraPosition(), 0);

	BaseClass::UpdateAll();
	BaseClass::DrawAll();

	//EnemyPool::UpdateAll();
	//EnemyPool::DrawAll();

	gState.miniScreen->Update();
	gState.miniScreen->Draw();

	// UI Stuff --- could maybe be put into its own class

	// Lines for the UI boarders
	int linePadding{ 10 };
	for (int x = 0; x < DISPLAY_WIDTH; x++)
	{
		Pixel colorUI{ 00,00,100 };
		for (int y = 0; y < linePadding; y++)
			graphics.DrawPixel({ x, MINI_HEIGHT + y }, colorUI);
		if (x == (DISPLAY_WIDTH / 2) - (MINI_WIDTH / 2) - linePadding || x == (DISPLAY_WIDTH / 2) + (MINI_WIDTH / 2))
			for (int h = MINI_HEIGHT; h > 0; h--)
				for (int ox = 0; ox < linePadding; ox++)
					graphics.DrawPixel({ x + ox, h }, colorUI);
	}

	// score
	graphics.DrawStringCentred(graphics.GetSpriteId("defenderblue45px_10x10"), { (DISPLAY_WIDTH / 2) + MINI_WIDTH, MINI_HEIGHT / 2 }, "SCORE ");

	// Sprites for the UI lives and bombs
	float livesSpaceing = 70; // 60 is the width of the sprite
	for (int l = 0; l < gState.lives; l++)
		graphics.Draw(Play::GetSpriteId("ship_right_1"), { livesSpaceing + (livesSpaceing * l), MINI_HEIGHT / 2 }, 0);

	float BombSpaceing = 22; // 12 is the height of the sprite
	for (int b = 0; b < gState.bombs; b++)
		graphics.Draw(Play::GetSpriteId("smartbomb_1"), { gState.miniScreen->GetPosition().x - 24 /* 24 is the width of the bomb sprite*/, (MINI_HEIGHT - linePadding) - (BombSpaceing * b) }, 0);

	// END of UI stuff

	gState.miniScreen->Update();
	gState.miniScreen->Draw();

	PlayWindow::Instance().Present();

	//BaseClass::CleanUp();

	// Return true to quit when escape key is pressed
	return PlayInput::Instance().KeyPressed(VK_ESCAPE);
}

// Gets called once when the player quits the game 
int MainGameExit(void)
{
	PlayGraphics::Destroy();
	PlayAudio::Destroy();
	PlayWindow::Destroy();
	PlayInput::Destroy();

	BaseClass::DeleteAll();

	return PLAY_OK;
}

void UpdateCamera()
{
	std::vector<BaseClass*> player = BaseClass::GetAllObjectsByType(BaseClass::PLAYER);
	Player* p = static_cast<Player*>(player[0]);
	Point2f cameraTargetPos;
	if (p->GetDirection() == Player::RIGHT)
		cameraTargetPos = { p->GetPosition().x - cameraOffset.x, 0 };
	else if (p->GetDirection() == Player::LEFT)
		cameraTargetPos = { (p->GetPosition().x - DISPLAY_WIDTH) + cameraOffset.x, 0 };

	Vector2f movement = (cameraTargetPos - Play::GetCameraPosition()) / cameraSpeed;
	Play::SetCameraPosition(Play::GetCameraPosition() + movement);
}

void GetBackground(int spriteWidth, int spriteHight, std::string spriteName)
{
	gState.backgroundID = Background::CreateBackground(spriteWidth, spriteHight, spriteName);
}