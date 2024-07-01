#include "stdafx.h"
#include "InformationUi.h"
#include "Game.h"

InformationUi::InformationUi()
{

}

InformationUi::~InformationUi()
{
	for (const auto& ptr : m_informationList)
	{
		delete ptr.second;
	}
}

bool InformationUi::Start()
{
	InformationSprite* info1 = new InformationSprite;
	info1->InitInformationSpriteParam("Assets/modelData/ui_information/Mission.DDS", 1920.0f / 2, 1080.0f / 2);
	InitInformationList("info1", info1);

	InformationSprite* info2 = new InformationSprite;
	info2->InitInformationSpriteParam("Assets/modelData/ui_information/Sousa.DDS", 1920.0f / 2, 1080.0f / 2);
	InitInformationList("info2", info2);

	m_game = FindGO<Game>("game");

	return true;
}

void InformationUi::Update()
{
	if (!m_isInfoDraw)
	{
		return;
	}

	WipeCalc();

	m_informationListIterator->second->m_InformationUi.Update();
}

void InformationUi::WipeCalc()
{

	if (m_isInfoWipe && GetNowIteratorWipeSizeX() >= -1920.0f)
	{
		m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeX -= 39.0f;

		m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeY -= 23.0f;

		return;
	}

	if (!m_isInfoWipe) 
	{
		m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeX += 39.0f;

		m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeY += 23.0f;

		if (GetNowIteratorWipeSizeX() >= 0)
		{
			m_isInfoDraw = false;
			m_isInfoWipe = true;
		}
	}

}

void InformationUi::Render(RenderContext& rc)
{
	if (!m_isInfoDraw)
	{
		return;
	}

	m_informationListIterator->second->m_InformationUi.Draw(rc);
}

