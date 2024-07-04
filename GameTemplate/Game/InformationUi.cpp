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

namespace {
	float WIDTH_WIPE_SPEED = 39.0f;
	float HEIGHT_WIPE_SPEED = 23.0f;
}

bool InformationUi::Start()
{
	//InformationSprite* info_Mission = new InformationSprite;
	//info_Mission->InitInformationSpriteParam("Assets/modelData/ui_information/Mission.DDS", 1920.0f / 2, 1080.0f / 2);
	//InitInformationList("Mission", info_Mission);

	//InformationSprite* info_Sousa = new InformationSprite;
	//info_Sousa->InitInformationSpriteParam("Assets/modelData/ui_information/Sousa.DDS", 1920.0f / 2, 1080.0f / 2);
	//InitInformationList("Sousa", info_Sousa);

	m_game = FindGO<Game>("game");

	return true;
}

void InformationUi::InitInformationSprite(const char* listName, const char* filePath, const float w, const float h)
{
	InformationSprite* new_Sprite = new InformationSprite;
	new_Sprite->InitInformationSpriteParam(filePath, w, h);
	InitInformationList(listName, new_Sprite);
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
		m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeX -= WIDTH_WIPE_SPEED;

		m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeY -= HEIGHT_WIPE_SPEED;

		if (GetNowIteratorWipeSizeX() <= -1920.0f)
		{
			m_isWipeFullyOpen = true;
		}

		return;
	}

	if (!m_isInfoWipe) 
	{
		m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeX += WIDTH_WIPE_SPEED;

		m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeY += HEIGHT_WIPE_SPEED;

		if (GetNowIteratorWipeSizeX() >= 0)
		{
			m_isInfoDraw = false;
			m_isInfoWipe = true;
			m_isWipeFullyOpen = false;
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

