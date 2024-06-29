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
	//NowLoding...を点滅させるDDSファイルを設定
	m_LoadMeianTexture1.InitFromDDSFile(L"Assets/sprite/GameLoadShader.DDS");
	initDataLoad.m_expandShaderResoruceView[0] = &m_LoadMeianTexture1;
	//Sprite表示用のシェーダーのファイルパスを指定する
	initDataLoad.m_fxFilePath = "Assets/shader/spriteLoad.fx";

	initDataLoad.m_expandConstantBuffer = &m_loadParam;
	initDataLoad.m_expandConstantBufferSize += sizeof(m_loadParam);
	//スプライトの幅と高さを指定する
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
	//アルファチャンネルの調整
	AlphaCalc();
	m_spriteLoad.Update();

	switch (m_state) {
	case enState_FadeIn: //薄まる
		m_loadParam.m_currentAlpha -= 0.50f * g_gameTime->GetFrameDeltaTime();
		if (m_loadParam.m_currentAlpha <= 0.0f) {
			m_loadParam.m_currentAlpha = 0.0f;
			m_state = enState_Idle;
		}
		break;
	case enState_FadeOut: //濃くなる
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

//アルファチャンネルの調整
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