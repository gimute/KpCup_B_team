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
	//ゲームタイマーを削除する
	DeleteGO(m_gametimer);
	//ゲームクリアのBGMを削除する
	DeleteGO(m_clearBgm);
}

bool GameClear::Start()
{
	m_gametimer = FindGO<GameTimer>("gametimer");

	//ゲームクリアの画面を読み込む。
	m_spriterender.Init("Assets/sprite/Game_Clear2.dds", 1920.0f, 1080.0f);
	//リザルトを見えやすくするためにモヤモヤの画面を読み込む
	m_spriterender_moya.Init("Assets/modelData/maintimer/moya.DDS", 700.0f, 100.0f);
	m_spriterender_moya.SetPosition(Vector3{ 400.0f,-280.0f,0.0f });
	m_spriterender_moya.SetMulColor({ 0.0f,0.0f,0.0f,0.4f });
	m_spriterender_moya.Update();
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
		//Yボタンを押したら
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			m_isWaitFadeout = true;
			m_load->StartFadeOut();
		}
	}
	//クリアタイム表示　一時敵
	DisplayTime();
	////Bボタンが押されていたら。
	//if (g_pad[0]->IsTrigger(enButtonY))
	//{
	//	m_load = FindGO<Load>("load");
	//	m_load->StartFadeOut();
	//	//タイトルのオブジェクトを作成
	//	NewGO<Title>(0, "title");
	//	//自身を削除する。
	//	DeleteGO(this);
	//}

	//if (m_isWaitFadeout) {
	//	if (!m_load->IsFade()) {
	//		NewGO<Title>(0, "game");
	//		//自身を削除する
	//		DeleteGO(this);
	//	}
	//}
	//else {
	//	//Aボタンを押したら
	//	if (g_pad[0]->IsTrigger(enButtonA)) {
	//		m_isWaitFadeout = true;
	//		m_load->StartFadeOut();
	//	}
}

//クリアタイム表示
void GameClear::DisplayTime()
{
	
	//クリアタイムのデータを持ってくる
	if (!m_isSaveClearTime) {
		m_cleartimer = m_gametimer->m_timer;	//分
		m_clearminit = int(m_gametimer->m_minit);

		m_isSaveClearTime = true;
	}
	wchar_t wcsbuf[256];
	//ゲームクリアしたタイムを表示
	swprintf_s(wcsbuf, 256, L"TOTAL TIME  %02d:%02d", int(m_cleartimer), int(m_clearminit));
	//表示するテキストを設定
	m_fontRender.SetText(wcsbuf);
	//フォントの位置を設定
	m_fontRender.SetPosition(Vector3(100.0f, -300.0f, 0.0f));
	//フォントの大きさを設定
	m_fontRender.SetScale(2.0f);
	//フォントの色を設定
	m_fontRender.SetColor({ 1.0f,0.843f,0.0f,1.0f});
	
	//表示するテキストを設定
	m_fontRender_tensen.SetText(L"..................................");
	//フォントの位置を設定
	m_fontRender_tensen.SetPosition(Vector3(120.0f, -350.0f, 0.0f));
	//フォントの大きさを設定
	m_fontRender_tensen.SetScale(1.0f);
	//フォントの色を設定
	m_fontRender_tensen.SetColor({ 0.0f,0.0f,0.0f,0.7f });

}

//描画処理
void GameClear::Render(RenderContext& rc)
{
	m_spriterender.Draw(rc);
	m_spriterender_moya.Draw(rc);
	m_fontRender.Draw(rc);
	m_fontRender_tensen.Draw(rc);
}
