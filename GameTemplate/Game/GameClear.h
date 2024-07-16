#pragma once
#include "sound/SoundSource.h"
class Game;
class Load;
class GameClear : public IGameObject
{
public:
	GameClear();
	~GameClear();
	bool Start();
	void AlphaCalc();
	//更新処理
	void Update();
	//クリアタイム表示
	void DisplayTime();
	//描画処理
	void Render(RenderContext& rc);
	
	/// <summary>
	/// ゲームクリア画面で表示するクリアタイムを設定する
	/// </summary>
	/// <param name="minute">分数</param>
	/// <param name="seconds">秒数</param>
	void SetClearTime(int minute, float seconds);

	//メンバ変数
	SpriteRender m_spriterender;	//スプライトレンダー
	SpriteRender m_spriterender_moya;	//スプライトレンダー
	SpriteRender m_pressRender;		//Press A Button レンダー
	Game* m_game = nullptr;
	GameClear* m_gameclear;
	Load* m_load = nullptr;
	FontRender m_fontRender;
	FontRender m_fontRender_tensen;
	SoundSource* m_clearBgm;

	bool m_isWaitFadeout = false;
	bool m_isSaveClearTime = false;
	float m_clearsecond = 0.0f;
	int m_clearminute = 0.0f;

	//アルファチャンネルの調整用の変数
	float m_alpha = 0.1f;
	bool m_alphaCalcBool;
};

