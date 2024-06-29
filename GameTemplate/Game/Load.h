#pragma once
class Game;
class Title;
class Load : public IGameObject
{
public: 

	struct loadParam {
		float m_currentAlpha;
		float m_fontAlpha;
	};

	Load();
	~Load();
	bool Start();
	//描画関数
	void Render(RenderContext& rc);
	//更新処理
	void Update();
	//アルファチャンネルの調整
	void AlphaCalc();
	//フェードイン
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	//フェードアウト
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	//フェード中ならtrue。
	const bool IsFade() const
	{
		return m_state != enState_Idle;
	}
	
	//スプライトレンダー
	SpriteRender m_spriteLoad;

	//アルファチャンネルの調整用の変数
	float m_alpha = 0.1f;
	//float m_currentAlpha = 0.0f; //現在のα値。ここを変更すると画像が透明になる。
	//bool m_loadEffect;
	bool m_alphaCalcBool;
	bool m_loadDraw = false;
	enum EnState {
		enState_FadeIn,		//フェードイン中
		enState_FadeOut,	//フェードアウト中
		enState_Idle,		//アイドル中
	};
	//状態
	EnState m_state = enState_Idle;

	Title* m_title;

	loadParam m_loadParam;

	/// <summary>
	/// 危険信号用テクスチャその１
	/// </summary>
	Texture m_LoadMeianTexture1;


};

