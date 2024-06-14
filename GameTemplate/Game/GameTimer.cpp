#include "stdafx.h"
#include "GameTimer.h"
#include "Gameclear.h"
#include "Game.h"

GameTimer::GameTimer()
{

}

GameTimer::~GameTimer()
{

}

bool GameTimer::Start()
{
	m_game = FindGO<Game>("game");

	//スプライトレンダー設定
	m_spriteRender.Init("Assets/modelData/maintimer/maintimer.DDS", 250.0f, 100.0f);
	//スプライトレンダーの位置を設定
	m_spriteRender.SetPosition(Vector3{ -645.0f, -370.0f, 0.0f});
	//フォントの位置を設定
	m_fontRender.SetPosition(Vector3(-900.0f, -400.0f, 0.0f));
	//フォントの大きさを設定
	m_fontRender.SetScale(2.0f);
	//フォントの色を設定。
	m_fontRender.SetColor({ 255.0f,0.0f,0.0f,1.0f });
	
	return true;
}

void GameTimer::Update()
{
	//タイム増加処理
	IncreaseTimer();
	//描画処理
	m_spriteRender.Update();
}

void GameTimer::IncreaseTimer()
{
	float nowTime = m_minit;
	for (; nowTime >= 60.0f;) {
		nowTime -= 60.0f;
		m_minit = nowTime;
		m_timer++;
	}
	m_minit += g_gameTime->GetFrameDeltaTime();	// 1f=1/60秒
}

void GameTimer::FontSet()
{
	wchar_t wcsbuf[256];
	//制限時間を表示
	swprintf_s(wcsbuf, 256, L"%02d:%02d", int(m_timer), int(m_minit));
	//表示するテキストを設定
	m_fontRender.SetText(wcsbuf);
	//フォントの位置を設定
	m_fontRender.SetPosition(Vector3(-900.0f, -400.0f, 0.0f));
	//フォントの大きさを設定
	m_fontRender.SetScale(2.0f);
	//フォントの色を設定
	m_fontRender.SetColor({ 1.0f,0.0f,0.0f,1.0f });

		//wchar_t wcsbuf[256];
		//swprintf_s(wcsbuf, 256, L"%02d:%02d", int(m_timer), int(m_minit));

		////表示するテキストを設定。
		//m_fontRender.SetText(wcsbuf);
}

void GameTimer::Render(RenderContext& rc)
{
	//ゲームオブジェクトが見つからなければ処理を飛ばす
	if (m_game == nullptr)
	{
		return;
	}
	if (m_game->m_TempDelGameTimer == true) {
		m_spriteRender.Draw(rc);
		m_fontRender.Draw(rc);
	}
}
