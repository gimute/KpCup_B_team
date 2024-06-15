#pragma once
class Game;
class GameClear;

class GameTimer:public IGameObject
{
public:
	GameTimer();
	~GameTimer();

	bool Start();
	//更新処理
	void Update();
	//タイム経過処理
	void IncreaseTimer();
	//
	void FontSet();
	//描画処理
	void Render(RenderContext& rc);

	//メンバ変数
	SpriteRender m_spriteRender;	//スプライトレンダー
	FontRender m_fontRender;		//フォントレンダー。
	Game* m_game;					//ゲーム
	GameClear* m_gameclear;			//ゲームクリア

	bool m_isSaveClearTime = false;

	float m_minit = 0.0f;	//秒数
	int m_timer = 0.0f;		//分数
};

