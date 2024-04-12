#include "stdafx.h"
#include "MainTimer.h"


namespace {
	const Vector3 n_timerpos100 = { -900.0f,-320.0f,0.0f };
	const Vector3 n_timerpos10 = { -820.0f,-320.0f,0.0f };
	const Vector3 n_timerpos1 = { -740.0f,-320.0f,0.0f };
}

//1920.0f, 1080.0f
bool MainTimer::Start()
{
	//スプライトレンダー設定
	m_spriterender.Init("Assets/modelData/maintimer/maintimer.DDS", 300.0f, 200.0f);
	//スプライトレンダーの位置を設定
	m_spriterender.SetPosition(m_uiposition);
	//フォントの位置を設定。
	m_fontrender.SetPosition(m_timerposition);
	//フォントの大きさを設定。
	m_fontrender.SetScale(m_timerscale);
	//フォントの色を設定。
	m_fontrender.SetColor(m_timercolor);
	//m_1timer初期設定
	m_1timer = m_1time;
	return true;
}

void MainTimer::Update()
{
	//タイム減少処理
	ReductionTimer();
	//タイム位置調整処理
	TimerPositionUpdate();
	//描画文字設定
	FontDrow();
	//描画処理
	m_spriterender.Update();
}

void MainTimer::ReductionTimer()
{
	//1秒待機
	if (m_reductiontime_bool) {
		m_1timer -= g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_1timer = m_1time;
	}

	//待機終了でタイマーを1減らす
	if (m_1timer < 0) {
		m_timer -= 1;
		m_1timer = m_1time;
	}

	//タイマーが0秒になった時の処理
	if (m_timer < 0) {
		//ここにプログラムを追加
		DeleteGO(this);
	}
}

void MainTimer::TimerPositionUpdate()
{
	//100秒以上の時の位置調整
	if (m_timer >= 100) {
		m_fontrender.SetPosition(n_timerpos100);
		return;
	}

	//100～10秒の時の位置調整
	if (m_timer < 100 && m_timer >= 10) {
		m_fontrender.SetPosition(n_timerpos10);
		return;
	}

	//10～0秒の時の位置調整
	if (m_timer < 10) {
		m_fontrender.SetPosition(n_timerpos1);
		return;
	}
}

void MainTimer::FontDrow()
{
	wchar_t wcsbuf[256];

	swprintf_s(wcsbuf, 256, L"%01d", int(m_timer));

	//表示するテキストを設定。
	m_fontrender.SetText(wcsbuf);
}

void MainTimer::Render(RenderContext& rc)
{
	m_spriterender.Draw(rc);
	m_fontrender.Draw(rc);
}