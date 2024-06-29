#include "stdafx.h"
#include "Load.h"
#include "Game.h"
#include "GameClear.h"
#include "Title.h"

Load::Load()
{

}

Load::~Load()
{

}

bool Load::Start()
{
	SpriteInitData initDataLoad;
	initDataLoad.m_ddsFilePath[0] = "Assets/sprite/GameLoad.DDS";
	//NowLoding...��_�ł�����DDS�t�@�C����ݒ�
	m_LoadMeianTexture1.InitFromDDSFile(L"Assets/sprite/GameLoadShader.DDS");
	initDataLoad.m_expandShaderResoruceView[0] = &m_LoadMeianTexture1;
	//Sprite�\���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
	initDataLoad.m_fxFilePath = "Assets/shader/spriteLoad.fx";

	initDataLoad.m_expandConstantBuffer = &m_loadParam;
	initDataLoad.m_expandConstantBufferSize += sizeof(m_loadParam);
	//�X�v���C�g�̕��ƍ������w�肷��
	initDataLoad.m_width = static_cast<UINT>(1920);
	initDataLoad.m_height = static_cast<UINT>(1080);
	initDataLoad.m_alphaBlendMode = AlphaBlendMode_Trans;
	

	m_spriteLoad.Init(initDataLoad);
	m_spriteLoad.SetPosition({ 0.0f,0.0f,0.0f });
	m_spriteLoad.Update();
	////////////////////////////////////////////////////////////////////
	//m_spriteLoad.Init("Assets/sprite/GameLoading.DDS",1920.0f,1080.0f);
	//m_spriteLoad.SetPosition({ 0.0f,0.0f,0.0f });
	//m_spriteLoad.Update();
	
	return true;
}

void Load::Update()
{
	//�A���t�@�`�����l���̒���
	AlphaCalc();
	m_spriteLoad.Update();

	switch (m_state) {
	case enState_FadeIn: //���܂�
		m_loadParam.m_currentAlpha -= 0.50f * g_gameTime->GetFrameDeltaTime();
		if (m_loadParam.m_currentAlpha <= 0.0f) {
			m_loadParam.m_currentAlpha = 0.0f;
			m_state = enState_Idle;
		}
		break;
	case enState_FadeOut: //�Z���Ȃ�
		m_loadParam.m_currentAlpha += 0.5f * g_gameTime->GetFrameDeltaTime();
		if (m_loadParam.m_currentAlpha >= 1.0f) {
			m_loadParam.m_currentAlpha = 1.0f;
			m_state = enState_Idle;
		}
		break;
	case enState_Idle:
		break;
	}
}

//�A���t�@�`�����l���̒���
void Load::AlphaCalc()
{
	if (m_loadParam.m_fontAlpha >= 1.0f) {
		m_alphaCalcBool = false;
	}

	if (m_alphaCalcBool)
	{
		m_loadParam.m_fontAlpha += 0.02f;
		return;
	}

	m_loadParam.m_fontAlpha -= 0.05f;
	
	if (m_loadParam.m_fontAlpha <= 0.1f)
	{
		m_alphaCalcBool = true;
	}

}

void Load::Render(RenderContext& rc)
{
	if (m_loadParam.m_currentAlpha > 0.0f) {
		m_spriteLoad.Draw(rc);
	}
	
}