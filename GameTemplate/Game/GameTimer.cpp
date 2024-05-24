#include "stdafx.h"
#include "GameTimer.h"
#include "Gameclear.h"

GameTimer::GameTimer()
{

}

GameTimer::~GameTimer()
{

}

bool GameTimer::Start()
{
	//�t�H���g�̈ʒu��ݒ�
	m_fontRender.SetPosition(Vector3(-900.0f, -400.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�
	m_fontRender.SetScale(2.0f);
	//�t�H���g�̐F��ݒ�B
	m_fontRender.SetColor({ 255.0f,0.0f,0.0f,1.0f });
	
	return true;
}

void GameTimer::Update()
{
	//�^�C����������
	IncreaseTimer();
}

void GameTimer::IncreaseTimer()
{
	float nowTime = m_minit;
	for (; nowTime >= 60.0f;) {
		nowTime -= 60.0f;
		m_minit = nowTime;
		m_timer++;
	}
	m_minit += g_gameTime->GetFrameDeltaTime();	// 1f=1/60�b
}

void GameTimer::FontDraw()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%02d:%02d", int(m_timer), int(m_minit));

	//�\������e�L�X�g��ݒ�B
	m_fontRender.SetText(wcsbuf);
}

void GameTimer::Render(RenderContext& rc)
{
	m_fontRender.Draw(rc);
}
