#pragma once
class Load : public IGameObject
{
public: 
	Load();
	~Load();
	bool Start();
	//描画関数
	void Render(RenderContext& rc);
	//更新処理
	void Update();
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
	SpriteRender m_spriteRender_load;

	//アルファチャンネルの調整用の変数
	float m_alpha = 0.1f;
	float m_currentAlpha = 0.0f; //現在のα値。ここを変更すると画像が透明になる。
	bool m_alphaCalcBool;

	enum EnState {
		enState_FadeIn,		//フェードイン中
		enState_FadeOut,	//フェードアウト中
		enState_Idle,		//アイドル中
	};
	//状態
	EnState m_state = enState_Idle;


};

