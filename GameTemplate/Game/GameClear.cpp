#include "stdafx.h"
#include "GameClear.h"
#include "Title.h"
#include "Game.h"

GameClear::GameClear()
{
	//ゲームクリアの画面を読み込む。
	m_spriterender.Init("Assets/sprite/Game_Clear2.dds", 1920.0f, 1080.0f);

}

GameClear::~GameClear()
{

}

//更新処理
void GameClear::Update()
{
	//Bボタンが押されていたら。
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		//タイトルのオブジェクトを作成
		NewGO<Title>(0, "title");
		//自身を削除する。
		DeleteGO(this);
	}
}

//描画処理
void GameClear::Render(RenderContext& rc)
{
	m_spriterender.Draw(rc);
}
