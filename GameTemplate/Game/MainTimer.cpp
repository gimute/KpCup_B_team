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
	//�X�v���C�g�����_�[�ݒ�
	m_spriterender.Init("Assets/modelData/maintimer/maintimer.DDS", 300.0f, 200.0f);
	//�X�v���C�g�����_�[�̈ʒu��ݒ�
	m_spriterender.SetPosition(m_uiposition);
	//�t�H���g�̈ʒu��ݒ�B
	m_fontrender.SetPosition(m_timerposition);
	//�t�H���g�̑傫����ݒ�B
	m_fontrender.SetScale(m_timerscale);
	//�t�H���g�̐F��ݒ�B
	m_fontrender.SetColor(m_timercolor);
	//m_1timer�����ݒ�
	m_1timer = m_1time;
	return true;
}

void MainTimer::Update()
{
	//�^�C����������
	ReductionTimer();
	//�^�C���ʒu��������
	TimerPositionUpdate();
	//�`�敶���ݒ�
	FontDrow();
	//�`�揈��
	m_spriterender.Update();
}

void MainTimer::ReductionTimer()
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

void MainTimer::TimerPositionUpdate()
{
	//100�b�ȏ�̎��̈ʒu����
	if (m_timer >= 100) {
		m_fontrender.SetPosition(n_timerpos100);
		return;
	}

	//100�`10�b�̎��̈ʒu����
	if (m_timer < 100 && m_timer >= 10) {
		m_fontrender.SetPosition(n_timerpos10);
		return;
	}

	//10�`0�b�̎��̈ʒu����
	if (m_timer < 10) {
		m_fontrender.SetPosition(n_timerpos1);
		return;
	}
}

void MainTimer::FontDrow()
{
	wchar_t wcsbuf[256];

	swprintf_s(wcsbuf, 256, L"%01d", int(m_timer));

	//�\������e�L�X�g��ݒ�B
	m_fontrender.SetText(wcsbuf);
}

void MainTimer::Render(RenderContext& rc)
{
	m_spriterender.Draw(rc);
	m_fontrender.Draw(rc);
}