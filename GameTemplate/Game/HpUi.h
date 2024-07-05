#pragma once
#define DecreaseTransitionIN0	m_decrease_TRAN = en_Standby_TRAN;
#define DecreaseTransitionIN1	m_decrease_TRAN = en_DelayTimeON_TRAN
#define DecreaseTransitionIN2	m_decrease_TRAN = en_DelayTimeLoop_TRAN;
#define DecreaseTransitionIN3	m_decrease_TRAN = en_TransparentDecreaseCalc_TRAN;
#define DecreaseTransitionIN4	m_decrease_TRAN = en_TransparentDecreaseON_TRAN;
#define DecreaseTransition0 m_decrease_TRAN == en_Standby_TRAN
#define DecreaseTransition1 m_decrease_TRAN == en_DelayTimeON_TRAN
#define DecreaseTransition2	m_decrease_TRAN == en_DelayTimeLoop_TRAN
#define DecreaseTransition3	m_decrease_TRAN == en_TransparentDecreaseCalc_TRAN
#define DecreaseTransition4	m_decrease_TRAN == en_TransparentDecreaseON_TRAN
class Game;
class HpUi : public IGameObject
{

public:
	//透過UI減少遷移
	enum DecreaseTransition {
		//待機0
		en_Standby_TRAN,
		//遅延開始1
		en_DelayTimeON_TRAN,
		//遅延処理2
		en_DelayTimeLoop_TRAN,
		//透過UI減少計算3
		en_TransparentDecreaseCalc_TRAN,
		//透過UI減少開始4
		en_TransparentDecreaseON_TRAN
	};
	/////////////////////////////////////////関数
	HpUi() {}
	~HpUi() {}
	//スタート関数
	bool Start();
	//アップデート
	void Update();
	//加減計算
	void Adjustment();
	//透過バー加減計算
	void AdjustmentTransparent();
	//レンダー
	void Render(RenderContext& rc);
	//HP減少処理
	void DecreaseHP(int decrease)
	{
		//HPを減少値減らす
		m_nowhp -= decrease;
		return;
	}

	int GetNowHP()
	{
		return m_nowhp;
	}

private:
	/////////////////////////////////////////メンバ変数
	//HPUI
	SpriteRender m_hpUI_A;
	//HPバー
	SpriteRender m_hpUI_B;
	//透過HPバー
	SpriteRender m_hpUI_C;
	//透過UI減少遷移
	DecreaseTransition m_decrease_TRAN = en_Standby_TRAN;
	//ゲーム
	Game* m_game = nullptr;
	/////////////////////////////////////////変数
	//最大HP
	int m_fullhp = 100;
	//現在HP　
	int m_nowhp = m_fullhp;
	//過去HP
	int m_oldhp = m_fullhp;
	//現在hpバースケール(動かすのはm_hpUI_B、zは固定)
	Vector3 m_scale_B = { 1.0,1.0,1.0 };
	//現在透過hpバースケール(動かすのはm_hpUI_C、zは固定)
	Vector3 m_scale_C = { 1.0,1.0,1.0 };
	//減少遅延タイマー
	float m_delaytimer = 0.0;
	//透過減少割合
	Vector3 m_changeS_C = m_scale_C;
	//現在透過減少値
	Vector3 m_nowScale = m_hpUI_C.GetScale();
	/////////////////////////////////////////初期設定系統
	
};

