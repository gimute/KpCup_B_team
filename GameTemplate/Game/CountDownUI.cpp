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
	switch (m_state)
	{
	case enStanby:
		break;

	case enThirdDraw:
		m_countDownTimer += g_gameTime->GetFrameDeltaTime();

		if (m_countDownTimer >= 1.0f)
		{
			m_countDownTimer = 0.0f;
			m_state = enSecondDraw;
		}
		break;

	case enSecondDraw:
		m_countDownTimer += g_gameTime->GetFrameDeltaTime();

		if (m_countDownTimer >= 1.0f)
		{
			m_countDownTimer = 0.0f;
			m_state = enFirsrDraw;
		}
		break;

	case enFirsrDraw:
		m_countDownTimer += g_gameTime->GetFrameDeltaTime();

		if (m_countDownTimer >= 1.0f)
		{
			m_countDownTimer = 0.0f;
			m_state = enStartDraw;
		}
		break;

	case enStartDraw:
		m_countDownTimer += g_gameTime->GetFrameDeltaTime();

		if (m_countDownTimer >= 1.0f)
		{
			m_countDownTimer = 0.0f;
			m_state = enEnd;
		}
		break;

	case enEnd:
		break;

	default:
		break;
	}

	
}


void CountDownUI::CountDownStart()
{
	//ステートをThirdDrawにしてカウントダウンを開始する
	m_state = enThirdDraw;
}

void CountDownUI::AlphaCalc()
{

}

void CountDownUI::Render(RenderContext& rc)
{
	switch (m_state)
	{
	case CountDownUI::enStanby:
		break;

	case CountDownUI::enThirdDraw:
		m_spriteRenderThird.Draw(rc);
		break;

	case CountDownUI::enSecondDraw:
		m_spriteRenderSecond.Draw(rc);
		break;

	case CountDownUI::enFirsrDraw:
		m_spriteRenderFirst.Draw(rc);
		break;

	case CountDownUI::enStartDraw:
		m_spriteRenderStart.Draw(rc);
		break;

	case CountDownUI::enEnd:
		break;

	default:
		break;
	}

	////if (!m_isCountDraw)	//カウントダウン描画を制御している
	////{
	////	return;
	////}
	//if (m_game->GetFirstCD()) {

	//	//if (CountDownUI::ThirdDraw)
	//	{
	//		m_minit += g_gameTime->GetFrameDeltaTime();
	//		if (m_minit <= 1.0f)
	//		{
	//			m_spriteRenderThird.Draw(rc);
	//		}
	//		else
	//		{
	//			m_isSecondDraw = true;
	//			//SetDrawState(CountDownUI::SecondDraw);
	//		}
	//		
	//	}
	//}

	//if (m_game->GetSecondCD()) {

	//	//if (CountDownUI::SecondDraw)
	//	if(m_isSecondDraw)
	//	{
	//		if (m_minit <= 2.0f)
	//		{
	//			m_spriteRenderSecond.Draw(rc);
	//		}
	//		else
	//		{
	//			m_isFirstDraw = true;
	//			//SetDrawState(CountDownUI::FirsrDraw);

	//		}
	//		
	//	}
	//}
	//
	//if (m_game->GetFirstCD()) {

	//	if (m_isFirstDraw)
	//	//if(CountDownUI::FirsrDraw == GetDrawState())
	//	{
	//		if (m_minit <= 3.0f)
	//		{
	//			m_spriteRenderFirst.Draw(rc);
	//		}
	//		else
	//		{
	//			m_isStartDraw = true;
	//			//SetDrawState(CountDownUI::StartD)
	//		}

	//	}
	//}
	//
	//if (m_game->GetStartCD()) {

	//	if (m_isStartDraw)
	//	{
	//		if (m_minit <= 4.0f)
	//		{
	//			m_spriteRenderStart.Draw(rc);
	//		}
	//		else
	//		{
	//			//m_isCountDownClose = true;
	//			SetCountDownClose(true);
	//		}
	//	}
	//}
}
