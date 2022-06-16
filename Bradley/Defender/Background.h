#pragma once
class Background
{
public:
	static std::vector<Background*> s_vBackground;
	//Background();
	//void Update();
	//void CallCreateBackground();
	static int CreateBackground(int spriteWidth, int spriteHight, std::string spriteName);
protected:
private:
	// variables for 1D
	static const int nOctaveCount{ 6 };
	int m_spriteID{ 0 };
};

