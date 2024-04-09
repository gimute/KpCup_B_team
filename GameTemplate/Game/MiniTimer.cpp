#include "stdafx.h"
#include "MiniTimer.h"


bool MiniTimer::Start()
{
	//�t�H���g�̈ʒu��ݒ�B
	m_fontrender.SetPosition(m_position);
	//�t�H���g�̑傫����ݒ�B
	m_fontrender.SetScale(m_scale);
	//�t�H���g�̐F��ݒ�B
	m_fontrender.SetColor(m_color);
	//m_1timer�����ݒ�
	m_1timer = m_1time;
	return true;
}

void MiniTimer::Update()
{
	//�^�C����������
	ReductionTimer();
	//�`�敶���ݒ�
	FontDrow();
}

void MiniTimer::ReductionTimer()
{
	//1�b�ҋ@
	if (m_reductiontime_bool) {
		m_1timer -= g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_1timer = m_1time;
	}

	//�ҋ@�I���Ń^�C�}�[��1���炷
	if (m_1timer < 0) {
		m_timer -= 1;
		m_1timer = m_1time;
	}

	//�^�C�}�[��0�b�ɂȂ������̏���
	if (m_timer < 0) {
		//�����Ƀv���O������ǉ�
		DeleteGO(this);
	}
}

void MiniTimer::FontDrow()
{
	wchar_t wcsbuf[256];

	swprintf_s(wcsbuf, 256, L"%01d", int(m_timer));

	//�\������e�L�X�g��ݒ�B
	m_fontrender.SetText(wcsbuf);
}

void MiniTimer::Render(RenderContext& rc)
{
	m_fontrender.Draw(rc);
}
