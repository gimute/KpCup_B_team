#pragma once
#include "Game.h"
#include "InformationUi.h"

class Game;
class InformationUi;

class CountDownUI : public IGameObject
{
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CountDownUI();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~CountDownUI();
	/// <summary>
	/// スタート関数
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// アップデート関数
	/// </summary>
	void Update();
	/// <summary>
	/// アルファ調整
	/// </summary>
	void AlphaCalc();
	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);

	bool GetFirstInfo()
	{
		return m_isFirstInfo;
	}

	void SetFirstInfo(bool FirstInfo)
	{
		m_isFirstInfo = FirstInfo;
		return;
	}

	bool GetSecondInfo()
	{
		return m_isSecondInfo;
	}

	void SetSecondInfo(bool SecondInfo)
	{
		m_isSecondInfo = SecondInfo;
	}
	
	bool GetThirdCoDown()
	{
		return m_isThirdCoDown;
	}

	void SetThirdCoDown(bool ThirdCoDown)
	{
		m_isThirdCoDown = ThirdCoDown;
	}

	bool GetSecondCoDown()
	{
		return m_isSecondCoDown;
	}

	void SetSecondCoDown(bool SecondCoDown)
	{
		m_isSecondCoDown = SecondCoDown;
	}

	bool GetFirstCoDown()
	{
		return m_isFirstCoDown;
	}

	void SetFirstCoDown(bool FirstCoDown)
	{
		m_isFirstCoDown = FirstCoDown;
	}

	bool GetStartCoDown()
	{
		return m_isStartCoDown;
	}

	void SetStartCoDown(bool StartCoDown)
	{
		m_isStartCoDown = StartCoDown;
	}

	bool GetCountDownClose()
	{
		return m_isCountDownClose;
	}

	void SetCountDownClose(bool CountDownClose)
	{
		m_isCountDownClose = CountDownClose;
	}


private:
	enum m_Draw
	{
		ThirdDraw,
		SecondDraw,
		FirsrDraw,
		StartDraw
	};
	SpriteRender	 m_spriteRenderThird;			// 「３」のスプライトレンダー
	SpriteRender	 m_spriteRenderSecond;			// 「２」のスプライトレンダー
	SpriteRender	 m_spriteRenderFirst;			// 「１」のスプライトレンダー
	SpriteRender	 m_spriteRenderStart;			// 「Start」のスプライトレンダー
	Game*			 m_game = nullptr;				// ゲームのインスタンス取得用変数
	GameTimer*		 m_gameTimer = nullptr;			// ゲームタイマーのインスタンス取得用変数
	InformationUi*	 m_infoUi = nullptr;			// インフォのインスタンス取得用変数
	bool			 m_isCountDraw = false;			// 描画するかどうか
	bool			 m_isCoDoWipe = false;			// ワイプの開閉
	bool			 m_isWipeFullOpen = false;		// ワイプが開ききったかどうか
	bool			 m_isSecondDraw = false;		// ２の画像を表示するかどうか
	bool			 m_isFirstDraw = false;			// １の画像を表示するかどうか
	bool			 m_isStartDraw = false;			// スタートの画像を表示するかどうか
	//bool			 m_isCountDownClose = false;	// すべてのカウントダウンの描画が閉じたかどうか
	bool			 m_isFirstInfo = false;			// １番目(ミッション)の画像が表示されているか
	bool			 m_isSecondInfo = false;		// ２番目(操作説明)の画像が表示されているか
	//m_Draw			 m_DrawState = ThirdDraw;

	bool			 m_isThirdCoDown = false;		// ３の画像が表示されているか
	bool			 m_isSecondCoDown = false;		// ２の画像が表示されているか
	bool			 m_isFirstCoDown = false;		// １の画像が表示されているか
	bool			 m_isStartCoDown = false;		// スタートの画像が表示されているか
	bool			 m_thirdCD = false;
	bool			 m_secondCD = false;
	bool			 m_firstCD = false;
	bool			 m_startCD = false;
	float			 m_minit = 0.0f;				// 秒数の変数
public:
	bool			 m_isCountDownClose = false;	// すべてのカウントダウンの描画が閉じたかどうか
	//m_Draw GetDrawState()
	//{
	//	return m_DrawState;
	//}
	//void SetDrawState(m_Draw State)
	//{
	//	m_DrawState = State;
	//}
};

