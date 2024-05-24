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
	void FontDraw();
	//描画処理
	void Render(RenderContext& rc);

	//メンバ変数
	SpriteRender m_spriteRender;	//スプライトレンダー
	FontRender m_fontRender;		//フォントレンダー。
	GameClear* m_gameclear;

	bool m_isSaveClearTime = false;

	float m_minit = 0.0f;	//秒数
	int m_timer = 0.0f;		//分数
};

