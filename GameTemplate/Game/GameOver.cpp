#include "stdafx.h"
#include "GameOver.h"
#include "Title.h"
#include "Game.h"
#include "Load.h"

GameOver::GameOver()
{
	
}

GameOver::~GameOver()
{

}

bool GameOver::Start()
{
	//�Q�[���I�[�o�[�̉�ʂ�ǂݍ��ށB
	m_spriteRender.Init("Assets/sprite/Game_Over2.dds", 1920.0f, 1080.0f);

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