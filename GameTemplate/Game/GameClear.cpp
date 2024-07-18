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
	DeleteGO(m_clearBgm);
}

bool GameClear::Start()
{
	//ゲームクリアの画面を読み込む。
	m_spriterender.Init("Assets/sprite/Game_Clear2.dds", 1920.0f, 1080.0f);
	//リザルトを見えやすくするためにモヤモヤの画面を読み込む
	m_spriterender_moya.Init("Assets/modelData/maintimer/moya.DDS", 700.0f, 100.0f);
	m_spriterender_moya.SetPosition(Vector3{ 400.0f,-280.0f,0.0f });
	m_spriterender_moya.SetMulColor({ 0.0f,0.0f,0.0f,0.4f });
	m_spriterender_moya.Update();
	//スプライトレンダー
	SpriteInitData initData;
	//Press Y Button の画像データを指定する
	initData.m_ddsFilePath[0] = "Assets/sprite/PressAButton2.dds";
	//Sprite表示用のシェーダーのファイルパスを指定する
	initData.m_fxFilePath = "Assets/shader/clear_pressA.fx";

	initData.m_expandConstantBuffer = &m_alpha;
	initData.m_expandConstantBufferSize += sizeof(float);
	
	//スプライトの幅と高さを指定する
	initData.m_width = static_cast<UINT>(453);
	initData.m_height = static_cast<UINT>(36);
	initData.m_alphaBlendMode = AlphaBlendMode_Trans;
	
	m_pressRender.Init(initData);
	m_pressRender.SetPosition({ -350.0f,-300.0f,0.0f });
	//m_pressRender.Init("Assets/sprite/PressYButton3.dds",453.0f,36.0f);
	//m_pressRender.SetPosition({ -350.0f,-300.0f,0.0f });
	m_pressRender.Update();

	//ゲームクリアのBGMを読み込む
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/m_clear.wav");
	//ゲームクリアのBGMを再生する
	m_clearBgm = NewGO<SoundSource>(3);
	m_clearBgm->Init(3);
	m_clearBgm->Play(true);

	m_load = FindGO<Load>("load");
	m_load->StartFadeIn();

	return true;
}

void GameClear::AlphaCalc()
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
void GameClear::Update()
{
	if (m_isWaitFadeout) {
		if (!m_load->IsFade()) {

				//タイトルのオブジェクトを作成
				NewGO<Title>(0, "title");
				//自身を削除する。
				DeleteGO(this);
		}
	}
	else {
		//Aボタンを押したら
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_isWaitFadeout = true;
			m_load->StartFadeOut();
		}
	}
	//クリアタイム表示　一時敵
	DisplayTime();

	AlphaCalc();
	m_pressRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	m_pressRender.Update();
}

//クリアタイム表示
void GameClear::DisplayTime()
{
	wchar_t wcsbuf[256];
	//ゲームクリアしたタイムを表示
	swprintf_s(wcsbuf, 256, L"TOTAL TIME  %02d:%02d", int(m_clearminute), int(m_clearsecond));
	//表示するテキストを設定
	m_fontRender.SetText(wcsbuf);
	//フォントの位置を設定
	m_fontRender.SetPosition(Vector3(100.0f, -280.0f, 0.0f));
	//フォントの大きさを設定
	m_fontRender.SetScale(1.0f);
	//フォントの色を設定
	m_fontRender.SetColor({ 1.0f,0.843f,0.0f,1.0f});
	
	//表示するテキストを設定
	m_fontRender_tensen.SetText(L"...................");
	//フォントの位置を設定
	m_fontRender_tensen.SetPosition(Vector3(80.0f, -300.0f, 0.0f));
	//フォントの大きさを設定
	m_fontRender_tensen.SetScale(1.0f);
	//フォントの色を設定
	m_fontRender_tensen.SetColor({ 0.0f,0.0f,0.0f,0.7f });

}

void GameClear::SetClearTime(int minute, float seconds)
{
	m_clearminute = minute;
	m_clearsecond = seconds;
}

//描画処理
void GameClear::Render(RenderContext& rc)
{
	m_spriterender.Draw(rc);
	m_spriterender_moya.Draw(rc);
	m_pressRender.Draw(rc);
	m_fontRender.Draw(rc);
	m_fontRender_tensen.Draw(rc);
}
