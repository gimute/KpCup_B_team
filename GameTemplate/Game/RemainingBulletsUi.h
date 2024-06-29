#pragma once

class Game;

class RemainingBulletsUi : public IGameObject
{
public:
	/////////////////////////////////////////構造体
	struct RemainingBulletReloadUiWipeParam
	{
		/// <summary>
		/// リロード表示スプライトのワイプ方向変数
		/// </summary>
		Vector2 m_remainingBulletReloadUiWipeDir;
		/// <summary>
		/// リロード表示スプライトのリニアワイプ変数
		/// </summary>
		float m_remainingBulletReloadUiWipeSize = 0.0f;
		/// <summary>
		/// フォントカラー
		/// </summary>
		float m_fontColor = 0.8f;
		/// <summary>
		/// 現在ワイプサイズ取得
		/// </summary>
		/// <returns></returns>
		float GetWipeSize()
		{
			return m_remainingBulletReloadUiWipeSize;
		}
		/// <summary>
		/// フォントカラー取得
		/// </summary>
		/// <returns></returns>
		float GetFontColor()
		{
			return m_fontColor;
		}
	};
	/////////////////////////////////////////関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	RemainingBulletsUi();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~RemainingBulletsUi();
	/// <summary>
	/// スタート関数
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// アップデート関数
	/// </summary>
	void Update();
	/// <summary>
	/// 残弾数描画処理
	/// </summary>
	void RemainingBulletNumberUiUpdate();
	/// <summary>
	/// ワイプ計算
	/// </summary>
	void WipeCalc();
	/// <summary>
	/// UIカラー計算
	/// </summary>
	void ColorCalc();
	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/////////////////////////////////////////変数
	/// <summary>
	/// 最大残弾数
	/// </summary>
	int m_RemainingBulletNumberFull = 0;
	/// <summary>
	/// リロード表示スプライトのワイプパラメーター
	/// </summary>
	RemainingBulletReloadUiWipeParam m_remainingBulletReloadUiWipeParam;
	/// <summary>
	/// リロード状態かどうか
	/// </summary>
	bool m_isReload = false;
	/// <summary>
	/// フォントカラー変更に使用するBool
	/// </summary>
	bool m_isFontColor = false;
	/////////////////////////////////////////スプライト
	/// <summary>
	///	残弾数ベーススプライト
	/// </summary>
	SpriteRender m_RemainingBulletUi;
	/// <summary>
	/// リロード表示スプライト
	/// </summary>
	SpriteRender m_RemainingBulletReloadUi;
	/// <summary>
	/// リロード表示テクスチャ
	/// </summary>
	Texture m_RemainingBulletReloadTexture;
	/// <summary>
	/// 残弾数スプライト
	/// </summary>
	FontRender m_RemainingBulletNumberUi;
	/////////////////////////////////////////ポインタ変数
	/// <summary>
	/// 残弾数
	/// </summary>
	int* m_RemainingBulletNumber;
	/// <summary>
	/// ゲームのインスタンスを格納するポインタ変数
	/// </summary>
	Game* m_game = nullptr;
	/////////////////////////////////////////メンバ関数
	/// <summary>
	/// 残弾アドレス登録
	/// </summary>
	/// <param name="bulletNum"></param>
	void InitRemainingBulletNum(int *bulletNum)
	{
		m_RemainingBulletNumber = bulletNum;
		m_RemainingBulletNumberFull = *m_RemainingBulletNumber;
	}
	/// <summary>
	/// リロード状態の変更
	/// </summary>
	/// <param name="isReload"></param>
	void IsReload(bool isReload)
	{
		m_isReload = isReload;
	}
};

