#include "stdafx.h"
#include "GameOver.h"
#include "Title.h"
#include "Game.h"
#include "Load.h"

GameOver::GameOver()
{
	
}

GameOver::~GameOver()
{

}

bool GameOver::Start()
{
	//ゲームオーバーの画面を読み込む。
	m_spriteRender.Init("Assets/sprite/Game_Over2.dds", 1920.0f, 1080.0f);

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