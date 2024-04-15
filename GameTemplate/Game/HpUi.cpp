#include "stdafx.h"
#include "HpUi.h"

namespace {
	const Vector3 n_position_A{ -600.0f,400.0f,0.0f };
	const Vector3 n_position_B{ -940.0f,400.0f,0.0f };
	const Vector2 n_pivot_B{ 0,0.5 };
}

bool HpUi::Start()
{
	//画像設定
	m_hpUI_A.Init("Assets/modelData/ui_hp/UI_A.DDS", 700.0f, 120.0f);
	m_hpUI_B.Init("Assets/modelData/ui_hp/UI_B.DDS", 670.0f, 90.0f);
	//位置設定
	m_hpUI_A.SetPosition(n_position_A);
	m_hpUI_B.SetPosition(n_position_B);
	//基点設定
	m_hpUI_B.SetPivot(n_pivot_B);
	return true;
}

void HpUi::Update()
{
	//加減計算処理
	Adjustment();
	//描画処理
	m_hpUI_A.Update();
	m_hpUI_B.Update();
}

void HpUi::Adjustment()
{
	//0以下にしない
	if (m_nowhp < 0) {
		m_nowhp = 0;
	}
	//現在HPが減少したら
	if (m_nowhp < m_oldhp)
	{
		float a = m_nowhp;
		float b = m_fullhp;
		//現在HPと最大HPの割合を計算
		float ratio = a / b;
		//現在のm_hpUI_Bのscale.xと割合を計算
		Vector3 m_changeS_B = m_scale_B;
		m_changeS_B.x = m_scale_B.x * ratio;
		m_hpUI_B.SetScale(m_changeS_B);
		//過去HPに現在のHPを保存
		m_oldhp = m_nowhp;
	}
}

void HpUi::Render(RenderContext& rc)
{
	//HPUIの描画処理を先に
	m_hpUI_A.Draw(rc);
	//バーの描画処理を後に
	m_hpUI_B.Draw(rc);
}