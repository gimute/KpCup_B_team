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

	//�X�v���C�g�����_�[�ݒ�
	m_spriteRender.Init("Assets/modelData/maintimer/maintimer.DDS", 250.0f, 100.0f);
	//�X�v���C�g�����_�[�̈ʒu��ݒ�
	m_spriteRender.SetPosition(Vector3{ -645.0f, -370.0f, 0.0f});
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
	//�`�揈��
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
	m_minit += g_gameTime->GetFrameDeltaTime();	// 1f=1/60�b
}

void GameTimer::FontSet()
{
	wchar_t wcsbuf[256];
	//�������Ԃ�\��
	swprintf_s(wcsbuf, 256, L"%02d:%02d", int(m_timer), int(m_minit));
	//�\������e�L�X�g��ݒ�
	m_fontRender.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�
	m_fontRender.SetPosition(Vector3(-900.0f, -400.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�
	m_fontRender.SetScale(2.0f);
	//�t�H���g�̐F��ݒ�
	m_fontRender.SetColor({ 1.0f,0.0f,0.0f,1.0f });

		//wchar_t wcsbuf[256];
		//swprintf_s(wcsbuf, 256, L"%02d:%02d", int(m_timer), int(m_minit));

		////�\������e�L�X�g��ݒ�B
		//m_fontRender.SetText(wcsbuf);
}

void GameTimer::Render(RenderContext& rc)
{
	//�Q�[���I�u�W�F�N�g��������Ȃ���Ώ������΂�
	if (m_game == nullptr)
	{
		return;
	}
	if (m_game->m_TempDelGameTimer == true) {
		m_spriteRender.Draw(rc);
		m_fontRender.Draw(rc);
	}
}
