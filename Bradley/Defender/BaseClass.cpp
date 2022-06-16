#include "PlayBufferPCH.h"
#include "MainGame.h"
#include "BaseClass.h"

std::vector<BaseClass*> BaseClass::s_vBaseClassObjectList;

BaseClass::BaseClass()
{
	s_vBaseClassObjectList.push_back(this);
}

BaseClass::~BaseClass()
{
	s_vBaseClassObjectList.erase(find(s_vBaseClassObjectList.begin(), s_vBaseClassObjectList.end(), this));
}

void BaseClass::UpdateAll()
{
	for (int i = 0; i < s_vBaseClassObjectList.size(); i++)
	{
		if (s_vBaseClassObjectList[i]->m_active == true)
			s_vBaseClassObjectList[i]->Update();
	}
}

void BaseClass::DrawAll()
{
	for (int i = 0; i < s_vBaseClassObjectList.size(); i++)
	{
		if (s_vBaseClassObjectList[i]->m_active == true)
			s_vBaseClassObjectList[i]->Draw();
	}
}

void BaseClass::SetSpriteID(const char* spriteName)
{
	PlayGraphics& graphics = PlayGraphics::Instance();
	m_spriteID = graphics.GetSpriteId(spriteName);
}

std::vector<BaseClass*> BaseClass::GetAllObjectsByType(Type type)
{
	std::vector<BaseClass*> vReturnList;
	for (int i = 0; i < s_vBaseClassObjectList.size(); i++)
	{
		if (s_vBaseClassObjectList[i]->m_type == type)
			vReturnList.push_back(s_vBaseClassObjectList[i]);
	}
	return vReturnList;
}

//void BaseClass::CleanUp()
//{
//	for (int d = 0; d < s_vBaseClassObjectList.size(); d++)
//		if (s_vBaseClassObjectList[d]->m_active == false)
//			delete(s_vBaseClassObjectList[d]);
//}

void BaseClass::DeleteAll()
{
	for (int d = 0; d < s_vBaseClassObjectList.size(); d++)
		delete(s_vBaseClassObjectList[d]);
}

bool BaseClass::HasCollision(BaseClass& obj1, BaseClass& obj2)
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