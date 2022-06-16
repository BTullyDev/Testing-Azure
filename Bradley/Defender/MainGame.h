#pragma once

constexpr int DISPLAY_WIDTH = 1280;
constexpr int DISPLAY_HEIGHT = 720;
constexpr int DISPLAY_SCALE = 1;

constexpr float MINI_DISPLAY_SCALE = 0.25f;
constexpr int MINI_WIDTH = DISPLAY_WIDTH * MINI_DISPLAY_SCALE;
constexpr int MINI_HEIGHT = DISPLAY_HEIGHT * MINI_DISPLAY_SCALE;

constexpr float LEVEL_WIDTH = DISPLAY_WIDTH * 4;
constexpr float LEVEL_HEIGHT = DISPLAY_HEIGHT; // <--- technically unneeded

constexpr int STARTING_LIVES = 2;
constexpr int MAX_LIVES = 5;

constexpr int STARTING_BOMBS = 3;
constexpr int MAX_BOMBS = 9;

class MiniScreen;

struct GameState
{
	float totalGameTime{ 0.0f };
	int backgroundID{ 0 };
	MiniScreen* miniScreen;
	float UIHeight{ 0 };
	int lives = STARTING_LIVES;
	int bombs = STARTING_BOMBS;
	int score = 123;

	//temp
	float* perlinNoise = new float[LEVEL_WIDTH];
};

// camera stuff
const Vector2f cameraOffset{ 140,0 };
const int cameraSpeed{ 12 }; // NOTE the higher the int the slower the camera moves -- 0 is instant movement

void UpdateCamera();
void GetBackground(int width, int height, std::string spriteName);