#pragma once
#include "Game.h"

class GameClear : public IGameObject
{
public:
	GameClear();
	~GameClear();
	//更新処理
	void Update();
	//描画処理
	void Render(RenderContext& rc);
	
	//メンバ変数
	SpriteRender m_spriterender;	//スプライトレンダー

	Game* m_game = nullptr;
	FontRender m_fontRender;
	GameClear* m_gameclear;
};

