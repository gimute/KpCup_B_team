#include "stdafx.h"
#include "SpeedMeter.h"
#include "Player.h"

SpeedMeter::SpeedMeter()
{

}

SpeedMeter::~SpeedMeter()
{

}

bool SpeedMeter::Start()
{

	m_player = FindGO<Player>("player");

	return true;
}

void SpeedMeter::Update()
{
	wchar_t wcsbuf[256];

	swprintf_s(wcsbuf, 256, L"SP:%01d", int(m_player->m_speed));

	//�\������e�L�X�g��ݒ�B
	m_fontrender.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_fontrender.SetPosition(Vector3(450.0f, 450.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_fontrender.SetScale(1.0f);
	//�t�H���g�̐F��ݒ�B
	m_fontrender.SetColor({ 0.0f,0.0f,0.0f,1.0f });
}

void SpeedMeter::Render(RenderContext& rc)
{
	m_fontrender.Draw(rc);
}
