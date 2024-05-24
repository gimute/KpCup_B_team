#include "stdafx.h"
#include "GameClear.h"
#include "Title.h"
#include "Game.h"
#include "Load.h"
#include "GameTimer.h"

GameClear::GameClear()
{

}

GameClear::~GameClear()
{

}

bool GameClear::Start()
{

	//ゲームクリアの画面を読み込む。
	m_spriterender.Init("Assets/sprite/Game_Clear2.dds", 1920.0f, 1080.0f);

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
	////クリアタイム表示　一時敵
	//DisplayTime();
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
	m_gametimer = FindGO<GameTimer>("gametimer");
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
	m_fontRender.SetColor({ 0.0f,0.0f,0.0f,1.0f});
}

//描画処理
void GameClear::Render(RenderContext& rc)
{
	m_spriterender.Draw(rc);

	m_fontRender.Draw(rc);
}
