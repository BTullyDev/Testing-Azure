#include "PlayBufferPCH.h"
#include "MainGame.h"
#include "Background.h"

std::vector<Background*> Background::s_vBackground;

//Background::Background()
//{
//	m_spriteID = Play::GetSpriteId(m_spriteName.c_str());
//	s_vBackground.push_back(this);
//}

int Background::CreateBackground(int spriteWidth, int spriteHight, std::string spriteName) // WARNING SHOULD ONLY BE CALLED ONCE
{
	extern GameState gState;
	PixelData NewBuffer{ spriteWidth, spriteHight, new Pixel[spriteWidth * spriteHight] };

	PlayGraphics& graphics = PlayGraphics::Instance();
	graphics.AddSprite(spriteName, NewBuffer);

	int width = NewBuffer.width;
	float* fSeed = new float[width];
	int nOctaves = nOctaveCount;
	float* fOutput = new float[width];


	// this generates the noise
	for (int i = 0; i < NewBuffer.width; i++)
		fSeed[i] = (float)Play::RandomRollRange(22'000, 30'000) / RAND_MAX;

	for (int x = 0; x < width; x++)
	{
		float fNoise = 0.0f;
		float fScale = 1.f;
		float fScaleAcc = 0.0f;
		float fLineSmooth = 0.5f; // <-- the higher the smother the line drawn

		for (int o = 0; o < nOctaves; o++)
		{
			int nPitch = width >> o; // >> o == / (2 * o)
			int nSample1 = (x / nPitch) * nPitch;
			int nSample2 = (nSample1 + nPitch) % width;

			float fBlend = (float)(x - nSample1) / (float)nPitch;
			float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];
			fNoise += fSample * fScale;

			fScaleAcc += fScale;
			fScale /= fLineSmooth;
		}

		//scale to seed range
		fOutput[x] = fNoise / fScaleAcc;
		gState.perlinNoise[x] = fNoise / fScaleAcc;
	}

	// this below is to draw the noise
	graphics.SetRenderTarget(&NewBuffer);
	graphics.ClearBuffer(Pixel(0x00, 0xFF, 0xFF, 0xFF));
	for (int x = 0; x < NewBuffer.width; x++)
	{
		int y = (NewBuffer.height * fOutput[x]);
		NewBuffer.pPixels[y * NewBuffer.width + x].r = 255;
		NewBuffer.pPixels[y * NewBuffer.width + x].g = 0;
		NewBuffer.pPixels[y * NewBuffer.width + x].b = 0;
		NewBuffer.pPixels[y * NewBuffer.width + x].a = 255;
	}

	graphics.UpdateSprite(spriteName, NewBuffer);

	graphics.SetRenderTarget(graphics.GetDrawingBuffer());

	delete[] fSeed;
	delete[] fOutput;
	return Play::GetSpriteId(spriteName.c_str());
}

//void Background::Update()
//{
//	PlayGraphics& graphics = PlayGraphics::Instance();
//
//	if (PlayInput::Instance().KeyPressed('W'))
//		nOctaveCount++;
//
//	if (nOctaveCount == 9)
//		nOctaveCount = 1;
//
//	graphics.Draw(m_spriteID, Point2f{ 0,0 } , 0);
//
//	//CreateBackground(nOutputSize, fNoiseSeed1D, nOctaveCount, fPerlinNoise1D);
//
//	//for (int x = 0; x < nOutputSize; x++)
//	//{
//	//	int y = -(fPerlinNoise1D[x] * DISPLAY_HEIGHT / 2) + DISPLAY_HEIGHT / 2;
//	//	for (int f = y; f < DISPLAY_HEIGHT / 2; f++)
//	//		graphics.DrawPixel({ x - Play::GetCameraPosition().x,f - Play::GetCameraPosition().y }, { 255, 255, 255 });
//	//}
//}
//
//void Background::CallCreateBackground()
//{
//	CreateBackground();
//}