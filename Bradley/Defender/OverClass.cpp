#include "PlayBufferPCH.h"
#include "MainGame.h"
#include "OverClass.h"

std::vector<OverClass*> OverClass::s_vOverClassObjectList;

OverClass::OverClass(Point2f pos)
{
	m_pos = pos;
	s_vOverClassObjectList.push_back(this);
}

OverClass::~OverClass()
{
	s_vOverClassObjectList.erase(find(s_vOverClassObjectList.begin(), s_vOverClassObjectList.end(), this));
}

void OverClass::UpdateAll()
{
	for (int i = 0; i < s_vOverClassObjectList.size(); i++)
	{
		s_vOverClassObjectList[i]->Update();
	}
}

void OverClass::DrawAll()
{
	for (int i = 0; i < s_vOverClassObjectList.size(); i++)
	{
		s_vOverClassObjectList[i]->Draw();
	}
}

void OverClass::SetSpriteID(const char* spriteName)
{
	PlayGraphics& graphics = PlayGraphics::Instance();
	m_spriteID = graphics.GetSpriteId(spriteName);
}

std::vector<OverClass*> OverClass::GetAllObjectsByType(Type type)
{
	std::vector<OverClass*> vReturnList;
	for (int i = 0; i < s_vOverClassObjectList.size(); i++)
	{
		if (s_vOverClassObjectList[i]->m_type == type)
			vReturnList.push_back(s_vOverClassObjectList[i]);
	}
	return vReturnList;
}

void OverClass::CleanUp()
{
	for (int d = 0; d < s_vOverClassObjectList.size(); d++)
		if (s_vOverClassObjectList[d]->m_active == false)
			delete(s_vOverClassObjectList[d]);
}

void OverClass::DeleteAll()
{
	for (int d = 0; d < s_vOverClassObjectList.size(); d++)
			delete(s_vOverClassObjectList[d]);
}

bool OverClass::HasCollision(OverClass& obj1, OverClass& obj2)
{
	Point2f pos1 = obj1.m_pos;
	Point2f pos2 = obj2.m_pos;
	float radius1 = obj1.m_collisionRadius;
	float radius2 = obj2.m_collisionRadius;
	if (pos1.x + (radius1 / 2) >= pos2.x - (radius2 / 2) && pos1.y + (radius1 / 2) >= pos2.y - (radius2 / 2) && pos1.y - (radius1 / 2) < pos2.y + (radius2 / 2) && pos1.x - (radius1 / 2) <= pos2.x + (radius2 / 2))
	return true;
	else
	return false;
}