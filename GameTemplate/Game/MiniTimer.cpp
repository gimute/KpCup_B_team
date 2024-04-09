#include "stdafx.h"
#include "MiniTimer.h"


bool MiniTimer::Start()
{
	//フォントの位置を設定。
	m_fontrender.SetPosition(m_position);
	//フォントの大きさを設定。
	m_fontrender.SetScale(m_scale);
	//フォントの色を設定。
	m_fontrender.SetColor(m_color);
	//m_1timer初期設定
	m_1timer = m_1time;
	return true;
}

void MiniTimer::Update()
{
	//タイム減少処理
	ReductionTimer();
	//描画文字設定
	FontDrow();
}

void MiniTimer::ReductionTimer()
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

void MiniTimer::FontDrow()
{
	wchar_t wcsbuf[256];

	swprintf_s(wcsbuf, 256, L"%01d", int(m_timer));

	//表示するテキストを設定。
	m_fontrender.SetText(wcsbuf);
}

void MiniTimer::Render(RenderContext& rc)
{
	m_fontrender.Draw(rc);
}
