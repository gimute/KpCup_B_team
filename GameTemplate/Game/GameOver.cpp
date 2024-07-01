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
	m_spriteRender.Init("Assets/sprite/Game_Over2.dds", UI_SPACE_WIDTH, UI_SPACE_HEIGHT);
	//�X�v���C�g�����_�[
	SpriteInitData initData;
	//Press Y Button �̉摜�f�[�^���w�肷��B
	initData.m_ddsFilePath[0] = "Assets/sprite/PressAButton2.dds";
	//Sprite�\���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��B
	initData.m_fxFilePath = "Assets/shader/over_pressA.fx";

	initData.m_expandConstantBuffer = &m_alpha;
	initData.m_expandConstantBufferSize += sizeof(float);

	//�X�v���C�g�̕��ƍ������w�肷��
	initData.m_width = static_cast <UINT>(453);
	initData.m_height = static_cast<UINT>(36);
	initData.m_alphaBlendMode = AlphaBlendMode_Trans;

	m_pressRender.Init(initData);
	m_pressRender.SetPosition({ 0.0f,-300.0f,0.0f });
	//m_pressRender.Init("Assets/sprite/PressYButton2.dds", 453.0f, 36.0f);
	//m_pressRender.SetPosition({ 0.0f,-300.0f,0.0f, });
	m_pressRender.Update();
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

void GameOver::AlphaCalc()
{
	if (m_alpha >= 1.0f)
	{
		m_alphaCalcBool = false;
	}

	if (m_alphaCalcBool)
	{
		m_alpha += 0.02f;
		return;
	}
	m_alpha -= 0.01f;

	if (m_alpha <= 0.1f)
	{
		m_alphaCalcBool = true;
	}
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
		//A�{�^����������Ă�����B
		if (g_pad[0]->IsTrigger(enButtonA))
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
	AlphaCalc();
	m_pressRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	m_pressRender.Update();
}

//�`�揈��
void GameOver::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	m_pressRender.Draw(rc);
}