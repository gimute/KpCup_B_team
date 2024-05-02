#pragma once
#include "Game.h"
#include "Enemy.h"
class Game;
class Enemy;
class Player;
class EnemyHpUi : public IGameObject
{
	/// <summary>
	/// 透過HPUI減少遷移
	/// </summary>
	enum DecreaseTransition {
		/// <summary>
		/// 待機0
		/// </summary>
		en_Standby_TRAN,
		/// <summary>
		/// 遅延開始1
		/// </summary>
		en_DelayTimeON_TRAN,
		/// <summary>
		/// 遅延処理2
		/// </summary>
		en_DelayTimeLoop_TRAN,
		/// <summary>
		/// 透過UI減少計算3
		/// </summary>
		en_TransparentDecreaseCalc_TRAN,
		/// <summary>
		/// 透過UI減少開始4
		/// </summary>
		en_TransparentDecreaseON_TRAN
	};
	/////////////////////////////////////////関数
public:
	EnemyHpUi() {};
	~EnemyHpUi() {};
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
	/// 距離表示計算
	/// </summary>
	void DisplayDis();
	/// <summary>
	/// 位置更新
	/// </summary>
	void PositionUpdate();
	/// <summary>
	/// 減算計算
	/// </summary>
	void Adjustment();
	/// <summary>
	/// 透過バー減算
	/// </summary>
	void AdjustmentTransparent();
	/// <summary>
	/// モデルレンダー
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/////////////////////////////////////////メンバ変数
	/// <summary>
	/// HPUI
	/// </summary>
	SpriteRender m_hpUI_A;
	/// <summary>
	/// HPバー
	/// </summary>
	SpriteRender m_hpUI_B;
	/// <summary>
	/// 透過HPバー
	/// </summary>
	SpriteRender m_hpUI_C;
	/// <summary>
	/// 透過HPバー減少遷移
	/// </summary>
	DecreaseTransition m_decrease_TRAN = en_Standby_TRAN;
	/// <summary>
	/// ゲーム
	/// </summary>
	Game* m_game = nullptr;
	/// <summary>
	/// プレイヤー
	/// </summary>
	Player* m_player = nullptr;
	/////////////////////////////////////////変数
	/// <summary>
	/// 最大HP
	/// </summary>
	int m_fullhp = 0;
	/// <summary>
	/// 現在HP
	/// </summary>
	int m_nowhp = m_fullhp;
	/// <summary>
	/// 過去HP
	/// </summary>
	int m_oldhp = m_fullhp;
	/// <summary>
	/// UIAの位置
	/// </summary>
	Vector2 m_positionA = { 0.0f,0.0f };
	/// <summary>
	/// UIBとCの位置
	/// </summary>
	Vector2 m_positionBC = { 0.0f,0.0f };
	/// <summary>
	/// 現在hpバースケール(動かすのはm_hpUI_B、zは固定)
	/// </summary>
	Vector3 m_scale_B = { 1.0,1.0,1.0 };
	/// <summary>
	/// 現在透過hpバースケール(動かすのはm_hpUI_C、zは固定)
	/// </summary>
	Vector3 m_scale_C = { 1.0,1.0,1.0 };
	/// <summary>
	/// 減少遅延タイマー
	/// </summary>
	float m_delaytimer = 0.0;
	/// <summary>
	/// 透過減少割合
	/// </summary>
	Vector3 m_changeS_C = m_scale_C;
	/// <summary>
	/// 現在透過減少値
	/// </summary>
	Vector3 m_nowScale = m_hpUI_C.GetScale();
	/// <summary>
	/// 配列のナンバー
	/// </summary>
	int m_Vectornum = 0;					
	/// <summary>
	/// 描画状態かそうでないか
	/// </summary>
	bool m_isImage = false;
	/////////////////////////////////////////初期設定系統
	/// <summary>
	/// HP減少処理
	/// </summary>
	/// <param name="decrease"></param>
	void DecreaseHP(int decrease)
	{
		//HPを減少値減らす
		m_nowhp -= decrease;
		return;
	}
	/// <summary>
	/// HP初期設定
	/// </summary>
	void SetEnemyHp()
	{
		m_fullhp = m_game->m_EnemyList[m_Vectornum]->m_hp;
		m_nowhp = m_fullhp;
		m_oldhp = m_fullhp;
	}
};

