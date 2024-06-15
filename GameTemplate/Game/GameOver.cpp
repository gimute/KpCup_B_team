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
	m_spriteRender.Init("Assets/sprite/Game_Over2.dds", 1920.0f, 1080.0f);
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
		//Yボタンが押されていたら。
		if (g_pad[0]->IsTrigger(enButtonY))
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
}

//描画処理
void GameOver::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}