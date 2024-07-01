#include "stdafx.h"
#include "InformationUi.h"

InformationUi::InformationUi()
{

}

InformationUi::~InformationUi()
{

}

bool InformationUi::Start()
{
	InformationSprite* info1 = new InformationSprite;
	info1->InitInformationSpriteParam("Assets/modelData/ui_information/Mission.DDS", 1920.0f / 2, 1080.0f / 2);
	InitInformationList("info1", info1);

	return true;
}

void InformationUi::Update()
{
	if (!m_isInfoDraw)
	{
		return;
	}

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_isInfoWipe = false;
	}

	WipeCalc();

	m_informationListIterator->second->m_InformationUi.Update();
}

void InformationUi::WipeCalc()
{

	if (m_isInfoWipe && GetNowIteratorWipeSize() >= -1920.0f)
	{
		m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeX -= 16.0f;

		m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeY -= 9.0f;

		return;
	}

	if (!m_isInfoWipe) 
	{
		m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeX += 16.0f;

		m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeY += 9.0f;

		if (GetNowIteratorWipeSize() >= 0)
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

