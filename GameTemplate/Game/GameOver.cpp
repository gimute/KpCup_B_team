#include "stdafx.h"
#include "GameOver.h"
#include "Title.h"
#include "Game.h"

GameOver::GameOver()
{
	//�Q�[���I�[�o�[�̉�ʂ�ǂݍ��ށB
	m_spriteRender.Init("Assets/sprite/Game_Over2.dds", 1920.0f, 1080.0f);
}

GameOver::~GameOver()
{

}

//�X�V����
void GameOver::Update()
{
	//B�{�^����������Ă�����B
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//�^�C�g���̃I�u�W�F�N�g���쐬
		NewGO<Title>(0, "title");
		//���g���폜����B
		DeleteGO(this);
	}
}

//�`�揈��
void GameOver::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}