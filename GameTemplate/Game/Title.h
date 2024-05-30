#pragma once

class Load;
// タイトル。
class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	//描画関数。
	void Render(RenderContext& rc);
	//更新処理。
	void Update();
	//アルファチャンネル調整
	void AlphaCalc();

	//スプライトレンダー。
	SpriteRender m_spriteRender_title;
	SpriteRender m_spriteRender_startButton;
	
	Load* m_load = nullptr;
	bool m_isWaitFadeout = false;

	//アルファチャンネルの調整用の変数
	float m_alpha = 0.1f;
	bool m_alphaCalcBool;
};

