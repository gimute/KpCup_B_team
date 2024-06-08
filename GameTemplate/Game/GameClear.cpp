#include "stdafx.h"
#include "GameClear.h"
#include "Title.h"
#include "Game.h"
#include "Load.h"
#include "GameTimer.h"
#include "sound/SoundEngine.h"

GameClear::GameClear()
{

}

GameClear::~GameClear()
{
	//�Q�[���^�C�}�[���폜����
	DeleteGO(m_gametimer);
	//�Q�[���N���A��BGM���폜����
	DeleteGO(m_clearBgm);
}

bool GameClear::Start()
{
	m_gametimer = FindGO<GameTimer>("gametimer");

	//�Q�[���N���A�̉�ʂ�ǂݍ��ށB
	m_spriterender.Init("Assets/sprite/Game_Clear2.dds", 1920.0f, 1080.0f);
	//���U���g�������₷�����邽�߂Ƀ��������̉�ʂ�ǂݍ���
	m_spriterender_moya.Init("Assets/modelData/maintimer/moya.DDS", 700.0f, 100.0f);
	m_spriterender_moya.SetPosition(Vector3{ 400.0f,-280.0f,0.0f });
	m_spriterender_moya.SetMulColor({ 0.0f,0.0f,0.0f,0.4f });
	m_spriterender_moya.Update();
	//�Q�[���N���A��BGM��ǂݍ���
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/m_clear.wav");
	//�Q�[���N���A��BGM���Đ�����
	m_clearBgm = NewGO<SoundSource>(3);
	m_clearBgm->Init(3);
	m_clearBgm->Play(true);

	m_load = FindGO<Load>("load");
	m_load->StartFadeIn();

	return true;
}

//�X�V����
void GameClear::Update()
{
	if (m_isWaitFadeout) {
		if (!m_load->IsFade()) {

				//�^�C�g���̃I�u�W�F�N�g���쐬
				NewGO<Title>(0, "title");
				//���g���폜����B
				DeleteGO(this);
		}
	}
	else {
		//Y�{�^������������
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			m_isWaitFadeout = true;
			m_load->StartFadeOut();
		}
	}
	//�N���A�^�C���\���@�ꎞ�G
	DisplayTime();
	////B�{�^����������Ă�����B
	//if (g_pad[0]->IsTrigger(enButtonY))
	//{
	//	m_load = FindGO<Load>("load");
	//	m_load->StartFadeOut();
	//	//�^�C�g���̃I�u�W�F�N�g���쐬
	//	NewGO<Title>(0, "title");
	//	//���g���폜����B
	//	DeleteGO(this);
	//}

	//if (m_isWaitFadeout) {
	//	if (!m_load->IsFade()) {
	//		NewGO<Title>(0, "game");
	//		//���g���폜����
	//		DeleteGO(this);
	//	}
	//}
	//else {
	//	//A�{�^������������
	//	if (g_pad[0]->IsTrigger(enButtonA)) {
	//		m_isWaitFadeout = true;
	//		m_load->StartFadeOut();
	//	}
}

//�N���A�^�C���\��
void GameClear::DisplayTime()
{
	
	//�N���A�^�C���̃f�[�^�������Ă���
	if (!m_isSaveClearTime) {
		m_cleartimer = m_gametimer->m_timer;	//��
		m_clearminit = int(m_gametimer->m_minit);

		m_isSaveClearTime = true;
	}
	wchar_t wcsbuf[256];
	//�Q�[���N���A�����^�C����\��
	swprintf_s(wcsbuf, 256, L"TOTAL TIME  %02d:%02d", int(m_cleartimer), int(m_clearminit));
	//�\������e�L�X�g��ݒ�
	m_fontRender.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�
	m_fontRender.SetPosition(Vector3(100.0f, -300.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�
	m_fontRender.SetScale(2.0f);
	//�t�H���g�̐F��ݒ�
	m_fontRender.SetColor({ 1.0f,0.843f,0.0f,1.0f});
	
	//�\������e�L�X�g��ݒ�
	m_fontRender_tensen.SetText(L"..................................");
	//�t�H���g�̈ʒu��ݒ�
	m_fontRender_tensen.SetPosition(Vector3(120.0f, -350.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�
	m_fontRender_tensen.SetScale(1.0f);
	//�t�H���g�̐F��ݒ�
	m_fontRender_tensen.SetColor({ 0.0f,0.0f,0.0f,0.7f });

}

//�`�揈��
void GameClear::Render(RenderContext& rc)
{
	m_spriterender.Draw(rc);
	m_spriterender_moya.Draw(rc);
	m_fontRender.Draw(rc);
	m_fontRender_tensen.Draw(rc);
}
