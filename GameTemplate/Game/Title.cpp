#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Load.h"

Title::Title()
{
	
}

Title::~Title()
{

}

bool Title::Start()
{
	//�^�C�g���̉摜
	m_spriteRender_title.Init("Assets/sprite/Game_Title5.DDS", 1920.0f, 1080.0f);

	SpriteInitData initData;
	//DDS�t�@�C���i�摜�f�[�^�j�̃t�@�C���p�X���w�肷��B
	//�X�^�[�g�{�^���̉摜�f�[�^���w�肷��B
	initData.m_ddsFilePath[0] = "Assets/sprite/Press_Start_Button2.DDS";

	//Sprite�\���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��B
	initData.m_fxFilePath = "Assets/shader/sprite_Title.fx";

	initData.m_expandConstantBuffer = &m_alpha;
	initData.m_expandConstantBufferSize += sizeof(float);

	//�X�v���C�g�̕��ƍ������w�肷��B
	initData.m_width = static_cast<UINT>(453);
	initData.m_height = static_cast<UINT>(36);
	initData.m_alphaBlendMode = AlphaBlendMode_Trans;
	
	m_spriteRender_startButton.Init(initData);
	m_spriteRender_startButton.SetPosition({0.0f, -300.0f, 0.0f});
	m_spriteRender_startButton.Update();
	
	m_load = FindGO<Load>("load");
	m_load->StartFadeIn();

	return true;
}

void Title::AlphaCalc()
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
	m_alpha -= 0.02f;
	if (m_alpha <= 0.1f)
	{
		m_alphaCalcBool = true;
	}
}

void Title::Update()
{
		//if (!m_load->IsFade()) {
		//	if (g_pad[0]->IsTrigger(enButtonA))
		//	{
		//		m_isWaitFadeout = true;
		//		m_load->StartFadeOut();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
		//		NewGO<Game>(0, "game");
		//		DeleteGO(this);
		//	}
		//}
	if (m_isWaitFadeout) {
		if (!m_load->IsFade()) {
			NewGO<Game>(0, "game");
			//���g���폜����
			DeleteGO(this);
		}
	}
	else {
		//A�{�^������������
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_isWaitFadeout = true;
			m_load->StartFadeOut();
		}
	}

	AlphaCalc();
	m_spriteRender_startButton.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	m_spriteRender_startButton.Update();
}


void Title::Render(RenderContext& rc)
{
	//�摜��`�悷��B
	m_spriteRender_title.Draw(rc);			//�^�C�g���̉摜
	m_spriteRender_startButton.Draw(rc);	//�X�^�[�g�{�^���̉摜
}
