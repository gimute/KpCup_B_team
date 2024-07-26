#include "stdafx.h"
#include "GameTimer.h"
#include "Gameclear.h"
#include "Game.h"
#include "MissionUI.h"

namespace{
	Vector2 SPRITE_SIZE = { 250.0f, 100.0f };
	//Vector3 SPRITE_POSITION = { -645.0f, -370.0f, 0.0f };
	Vector3 SPRITE_POSITION = { -700.0f, -400.0f, 0.0f };

	//Vector3 FONT_POSITION = { -900.0f, -400.0f, 0.0f };
	Vector3 FONT_POSITION = { -900.0f, -400.0f, 0.0f };
}

GameTimer::GameTimer()
{

}

GameTimer::~GameTimer()
{
	DeleteGO(m_missionUI);
}

bool GameTimer::Start()
{
	m_game = FindGO<Game>("game");

	//スプライトレンダー設定
	m_spriteRender.Init("Assets/modelData/maintimer/maintimer.DDS", SPRITE_SIZE.x, SPRITE_SIZE.y);
	//スプライトレンダーの位置を設定
	m_spriteRender.SetPosition(SPRITE_POSITION);
	//フォントの位置を設定
	m_fontRender.SetPosition(FONT_POSITION);
	//フォントの大きさを設定
	m_fontRender.SetScale(1.0f);
	//フォントの色を設定。
	m_fontRender.SetColor({ 255.0f,0.0f,0.0f,1.0f });
	
	m_missionUI = NewGO<MissionUI>(0, "missionui");

	m_missionUI->SetPosition(Vector3{ SPRITE_POSITION.x + SPRITE_SIZE.x / 2, SPRITE_POSITION.y - SPRITE_SIZE.y / 2, 0.0f });

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
	float nowTime = m_second;
	for (; nowTime >= 60.0f;) {
		nowTime -= 60.0f;
		m_second = nowTime;
		m_minute++;
	}
	m_second += g_gameTime->GetFrameDeltaTime();	// 1f=1/60秒
}

void GameTimer::FontSet()
{
	wchar_t wcsbuf[256];
	//制限時間を表示
	swprintf_s(wcsbuf, 256, L"%02d:%02d", int(m_minute), int(m_second));
	//表示するテキストを設定
	m_fontRender.SetText(wcsbuf);
	//フォントの位置を設定
	m_fontRender.SetPosition(Vector3(-900.0f, -400.0f, 0.0f));
	//フォントの大きさを設定
	m_fontRender.SetScale(1.0f);
	//フォントの色を設定
	m_fontRender.SetColor({ 1.0f,0.0f,0.0f,1.0f });
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
