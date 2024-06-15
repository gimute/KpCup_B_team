#include "stdafx.h"
#include "GameOver.h"
#include "Title.h"
#include "Game.h"
#include "Load.h"
#include "sound/SoundEngine.h"

GameOver::GameOver()
{
	
}

GameOver::~GameOver()
{
	//�Q�[���I�[�o�[��BGM���폜����
	DeleteGO(m_overBgm);
}

bool GameOver::Start()
{
	//�Q�[���I�[�o�[�̉�ʂ�ǂݍ��ށB
	m_spriteRender.Init("Assets/sprite/Game_Over2.dds", 1920.0f, 1080.0f);
	//�Q�[���I�[�o�[��BGM��ǂݍ���
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/m_over.wav");
	//�Q�[���I�[�o�[��BGM���Đ�����
	m_overBgm = NewGO<SoundSource>(3);
	m_overBgm -> Init(4);
	m_overBgm->Play(true);

	m_load = FindGO<Load>("load");
	m_load->StartFadeIn();

	return true;
}

//�X�V����
void GameOver::Update()
{
	if (m_isWaitFadeout) {
		if (!m_load->IsFade())
		{
			//�^�C�g���̃I�u�W�F�N�g���쐬
			NewGO<Title>(0, "title");
			//���g���폜����
			DeleteGO(this);
		}
	}
	else {
		//Y�{�^����������Ă�����B
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			m_isWaitFadeout = true;
			m_load->StartFadeOut();
		}
	}
	////B�{�^����������Ă�����B
	//if (g_pad[0]->IsTrigger(enButtonB))
	//{
	//	//�^�C�g���̃I�u�W�F�N�g���쐬
	//	NewGO<Title>(0, "title");
	//	//���g���폜����B
	//	DeleteGO(this);
	//}
}

//�`�揈��
void GameOver::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}