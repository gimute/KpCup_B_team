#pragma once
#include "sound/SoundSource.h"
class Game;
class GameTimer;
class Load;
class GameClear : public IGameObject
{
public:
	GameClear();
	~GameClear();
	bool Start();
	//更新処理
	void Update();
	//クリアタイム表示
	void DisplayTime();
	//描画処理
	void Render(RenderContext& rc);
	
	//メンバ変数
	SpriteRender m_spriterender;	//スプライトレンダー
	SpriteRender m_spriterender_moya;	//スプライトレンダー

	Game* m_game = nullptr;
	GameTimer* m_gametimer = nullptr;
	GameClear* m_gameclear;
	Load* m_load = nullptr;
	FontRender m_fontRender;
	FontRender m_fontRender_tensen;
	SoundSource* m_clearBgm;

	bool m_isWaitFadeout = false;
	bool m_isSaveClearTime = false;
	float m_clearminit = 0.0f;
	int m_cleartimer = 0.0f;
};

