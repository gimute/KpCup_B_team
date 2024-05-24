#pragma once

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

	Game* m_game = nullptr;
	GameTimer* m_gametimer = nullptr;
	FontRender m_fontRender;
	GameClear* m_gameclear;
	Load* m_load = nullptr;

	bool m_isWaitFadeout = false;
	bool m_isSaveClearTime = false;
	float m_clearminit = 0.0f;
	int m_cleartimer = 0.0f;
};

