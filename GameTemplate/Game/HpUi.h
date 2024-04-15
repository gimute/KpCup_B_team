#pragma once
class HpUi : public IGameObject
{

public:
	/////////////////////////////////////////関数
	HpUi(){}
	~HpUi(){}
	//スタート関数
	bool Start();
	//アップデート
	void Update();
	//加減計算
	void Adjustment();
	//レンダー
	void Render(RenderContext& rc);
	/////////////////////////////////////////メンバ変数
	//HPUI
	SpriteRender m_hpUI_A;
	//HPバー
	SpriteRender m_hpUI_B;
	/////////////////////////////////////////変数
	//最大HP
	int m_fullhp = 100;
	//現在HP　
	int m_nowhp = m_fullhp;
	//過去HP
	int m_oldhp = m_fullhp;
	//現在hpバースケール(動かすのはm_hpUI_B、zは固定)
	Vector3 m_scale_B = { 1.0,1.0,1.0 };
	/////////////////////////////////////////初期設定系統
	//HP減少処理
	void DecreaseHP(int decrease)
	{
		//HPを減少値減らす
		m_nowhp -= decrease;
		return;
	}
};

