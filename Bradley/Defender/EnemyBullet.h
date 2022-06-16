#pragma once
class EnemyBullet : public EnemyBaseClass
{
public:
	EnemyBullet(Point2f pos, Vector2f velocity);
	~EnemyBullet() {};
	void Update() override;
	void Draw() override;
	static void CreateEnemyBullet(Point2f pos, Vector2f velocity);

	float GetSpeed() { return m_speed; }
protected:
	const float m_speed{ 50 };
	const int m_radius{ 16 };
private:
};

