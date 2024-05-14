#include "k2EngineLowPreCompile.h"
#include "SpriteRender.h"

namespace nsK2EngineLow {
	void SpriteRender::Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode)
	{
		SpriteInitData initData;
		//DDSファイル(画像データ)のファイルパスを指定する。
		initData.m_ddsFilePath[0] = filePath;
		//Sprite表示用のシェーダーのファイルパスを指定する。
		initData.m_fxFilePath = "Assets/shader/sprite.fx";
		//スプライトの幅と高さを指定する。
		initData.m_width = static_cast<UINT>(w);
		initData.m_height = static_cast<UINT>(h);
		initData.m_alphaBlendMode = alphaBlendMode;

		//Sprite初期化オブジェクトを使用して、Spriteを初期化する。
		m_sprite.Init(initData);
	}

	void SpriteRender::InitFxModeChange(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode
		,const FxChange fxChange)
	{
		SpriteInitData initData;
		//DDSファイル(画像データ)のファイルパスを指定する。
		initData.m_ddsFilePath[0] = filePath;
		//Sprite表示用のシェーダーのファイルパスを指定する。
		switch (m_fxChange)
		{
		case nsK2EngineLow::SpriteRender::en_mode1:
			initData.m_fxFilePath = "Assets/shader/sprite.fx";
			break;
		case nsK2EngineLow::SpriteRender::en_modeNone:
			initData.m_fxFilePath = "Assets/shader/sprite.fx";
			break;
		default:
			initData.m_fxFilePath = "Assets/shader/sprite.fx";
			break;
		}
		//スプライトの幅と高さを指定する。
		initData.m_width = static_cast<UINT>(w);
		initData.m_height = static_cast<UINT>(h);
		initData.m_alphaBlendMode = alphaBlendMode;

		//Sprite初期化オブジェクトを使用して、Spriteを初期化する。
		m_sprite.Init(initData);
	}

	void SpriteRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->AddRenderObject(this);
	}

	void SpriteRender::OnRender2D(RenderContext& rc)
	{
		m_sprite.Draw(rc);
	}
}
