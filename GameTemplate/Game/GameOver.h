#pragma once
#include "sound/SoundSource.h"
class Game;
class Load;
class GameOver : public IGameObject 
{
public:
	GameOver();
	~GameOver();
	bool Start();
	//更新処理
	void Update();
	//描画処理
	void Render(RenderContext& rc);

	Game* m_game = nullptr;
	Load* m_load = nullptr;

	//メンバ変数
	SpriteRender m_spriteRender;	//スプライトレンダー
	SoundSource* m_overBgm;			//ゲームオーバーのBGM
	bool m_isWaitFadeout = false;
};

