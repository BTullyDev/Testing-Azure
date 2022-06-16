#pragma once
class ClassStates
{
public:
protected:
private:
};

class PlayerDirection : ClassStates
{
public:
	void setDirection(bool isMovingRight) { m_movingRight = isMovingRight; }
	bool GetDirection() { return m_movingRight; }
protected:
	bool m_movingRight{ true };
private:
};