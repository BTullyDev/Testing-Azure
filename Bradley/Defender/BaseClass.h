#pragma once

class BaseClass
{
public:
	static std::vector<BaseClass*> s_vBaseClassObjectList;

	enum Type
	{
		NONE = -1,
		PLAYER,
		LASER,
		STAR,
		ENEMYPOOL,

		//enemies
		LANDER,
		MUTANT,
		BAITER,
		BOMBER,
		POD,
		SWARMER,
		ENEMY_BULLET,

	};

	enum MoveDirection
	{
		LEFT = 1,
		RIGHT = 2,
	};

	BaseClass();
	virtual ~BaseClass();

	void SetPosition(Point2f position) { m_pos = position; }
	Point2f GetPosition() { return m_pos; }

	void SetVelocity(Vector2f vel) { m_velocity = vel; }
	Vector2f GetVelocity() { return m_velocity; }

	void SetSpriteID(const char* spriteName);

	void SetType(Type type) { m_type = type; }
	Type GetType() { return m_type; }

	void SetCollisionRadius(float collisionRadius) { m_collisionRadius = collisionRadius; }
	float GetCollisionRadius() { return m_collisionRadius; }

	int GetSpriteID() { return m_spriteID; }

	void SetActive(bool active) { m_active = active; }
	bool GetActive() { return m_active; }

	// virtual functions for overriding
	virtual void Draw() {};
	virtual void Update() {};

	// static functions
	static void DrawAll();
	static void UpdateAll();
	//static void CleanUp();
	static void DeleteAll();
	static std::vector<BaseClass*> GetAllObjectsByType(Type type);
	static bool HasCollision(BaseClass& obj1, BaseClass& obj2);
protected:
	Point2f m_pos{ 0,0 };
	int m_spriteID{ 0 };
	Type m_type{ NONE };
	Vector2f m_velocity{ 0,0 };
	bool m_active{ true };
	MoveDirection m_direction{ RIGHT };
	float m_collisionRadius{ 0 };
};