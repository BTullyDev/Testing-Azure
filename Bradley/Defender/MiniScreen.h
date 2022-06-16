#pragma once
class MiniScreen
{
	friend class BaseClass;
public:
	MiniScreen();
	~MiniScreen();
	void Update();
	void Draw();
	Vector2f GetPosition() { return m_pos; }
	PixelData GetMiniBuffer() { return MiniBuffer; }

	void CreateSprite();
protected:
	const Point2f m_pos{(DISPLAY_WIDTH / 2) - ((DISPLAY_WIDTH / 4) / 2), 0};
	PixelData MiniBuffer{ (DISPLAY_WIDTH / 4), DISPLAY_HEIGHT / 4, new Pixel[(DISPLAY_WIDTH / 4) * DISPLAY_HEIGHT / 4] };
	const std::string m_screenSpriteName{ "miniScreen" };
	const std::string m_mapSpriteName{ "miniMapBackground" };
	int m_mapSpriteID{ -1 };
};