#include "stdafx.h"
#include "CountDownUI.h"
#include "Game.h"
#include "GameTimer.h"
#include "InformationUi.h"

CountDownUI::CountDownUI()
{

}

CountDownUI::~CountDownUI()
{

}



bool CountDownUI::Start()
{
	m_spriteRenderThird.Init("Assets/modelData/ui_countDown/count_3.DDS", 960.0f, 540.0f);
	m_spriteRenderSecond.Init("Assets/modelData/ui_countDown/count_2.DDS", 960.0f, 540.0f);
	m_spriteRenderFirst.Init("Assets/modelData/ui_countDown/count_1.DDS", 960.0f, 540.0f);
	m_spriteRenderStart.Init("Assets/modelData/ui_countDown/count_Start.DDS", 960.0f, 540.0f);

	m_game = FindGO<Game>("game");
	return true;
}



void CountDownUI::Update()
{
	CountDownUI* Render();
	if (!m_isCountDraw)
	{
		return;
	}

	
}

void CountDownUI::AlphaCalc()
{

}

void CountDownUI::Render(RenderContext& rc)
{
	//if (!m_isCountDraw)	//カウントダウン描画を制御している
	//{
	//	return;
	//}
	if (m_game->GetFirstCD()) {

		//if (CountDownUI::ThirdDraw)
		{
			m_minit += g_gameTime->GetFrameDeltaTime();
			if (m_minit <= 1.0f)
			{
				m_spriteRenderThird.Draw(rc);
			}
			else
			{
				m_isSecondDraw = true;
				//SetDrawState(CountDownUI::SecondDraw);
			}
			
		}
	}

	if (m_game->GetSecondCD()) {

		//if (CountDownUI::SecondDraw)
		if(m_isSecondDraw)
		{
			if (m_minit <= 2.0f)
			{
				m_spriteRenderSecond.Draw(rc);
			}
			else
			{
				m_isFirstDraw = true;
				//SetDrawState(CountDownUI::FirsrDraw);

			}
			
		}
	}
	
	if (m_game->GetFirstCD()) {

		if (m_isFirstDraw)
		//if(CountDownUI::FirsrDraw == GetDrawState())
		{
			if (m_minit <= 3.0f)
			{
				m_spriteRenderFirst.Draw(rc);
			}
			else
			{
				m_isStartDraw = true;
				//SetDrawState(CountDownUI::StartD)
			}

		}
	}
	
	if (m_game->GetStartCD()) {

		if (m_isStartDraw)
		{
			if (m_minit <= 4.0f)
			{
				m_spriteRenderStart.Draw(rc);
			}
			else
			{
				//m_isCountDownClose = true;
				SetCountDownClose(true);
			}
		}
	}
}
