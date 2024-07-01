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
	//ゲームオーバーのBGMを削除する
	DeleteGO(m_overBgm);
}

bool GameOver::Start()
{
	//ゲームオーバーの画面を読み込む。
	m_spriteRender.Init("Assets/sprite/Game_Over2.dds", UI_SPACE_WIDTH, UI_SPACE_HEIGHT);
	//スプライトレンダー
	SpriteInitData initData;
	//Press Y Button の画像データを指定する。
	initData.m_ddsFilePath[0] = "Assets/sprite/PressAButton2.dds";
	//Sprite表示用のシェーダーのファイルパスを指定する。
	initData.m_fxFilePath = "Assets/shader/over_pressA.fx";

	initData.m_expandConstantBuffer = &m_alpha;
	initData.m_expandConstantBufferSize += sizeof(float);

	//スプライトの幅と高さを指定する
	initData.m_width = static_cast <UINT>(453);
	initData.m_height = static_cast<UINT>(36);
	initData.m_alphaBlendMode = AlphaBlendMode_Trans;

	m_pressRender.Init(initData);
	m_pressRender.SetPosition({ 0.0f,-300.0f,0.0f });
	//m_pressRender.Init("Assets/sprite/PressYButton2.dds", 453.0f, 36.0f);
	//m_pressRender.SetPosition({ 0.0f,-300.0f,0.0f, });
	m_pressRender.Update();
	//ゲームオーバーのBGMを読み込む
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/m_over.wav");
	//ゲームオーバーのBGMを再生する
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

//更新処理
void GameOver::Update()
{
	if (m_isWaitFadeout) {
		if (!m_load->IsFade())
		{
			//タイトルのオブジェクトを作成
			NewGO<Title>(0, "title");
			//自身を削除する
			DeleteGO(this);
		}
	}
	else {
		//Aボタンが押されていたら。
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_isWaitFadeout = true;
			m_load->StartFadeOut();
		}
	}
	////Bボタンが押されていたら。
	//if (g_pad[0]->IsTrigger(enButtonB))
	//{
	//	//タイトルのオブジェクトを作成
	//	NewGO<Title>(0, "title");
	//	//自身を削除する。
	//	DeleteGO(this);
	//}
	AlphaCalc();
	m_pressRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	m_pressRender.Update();
}

//描画処理
void GameOver::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	m_pressRender.Draw(rc);
}