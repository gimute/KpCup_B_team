#include "stdafx.h"
#include "HpUi.h"
///
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
/// </summary>

namespace {
	//Aの位置
	const Vector3 n_position_A{ -440.0f,350.0f,0.0f };
	//BとCの位置
	const Vector3 n_position_BC{ -765.0f,350.0f,0.0f };
	//BとCの基点
	const Vector2 n_pivot_BC{ 0,0.5 };
	//delaytimerの初期設定時間
	float m_delaytime = 0.7f;
}

bool HpUi::Start()
{
	//画像設定
	m_hpUI_A.Init("Assets/modelData/ui_hp/UI_A.DDS", 670.0f, 90.0f);
	m_hpUI_B.Init("Assets/modelData/ui_hp/UI_B.DDS", 630.0f, 60.0f);
	m_hpUI_C.Init("Assets/modelData/ui_hp/UI_C.DDS", 630.0f, 60.0f);
	//位置設定
	m_hpUI_A.SetPosition(n_position_A);
	m_hpUI_B.SetPosition(n_position_BC);
	m_hpUI_C.SetPosition(n_position_BC);
	//基点設定
	m_hpUI_B.SetPivot(n_pivot_BC);
	m_hpUI_C.SetPivot(n_pivot_BC);
	return true;
}

void HpUi::Update()
{
	//減算計算処理
	Adjustment();
	//透過減算計算処理
	AdjustmentTransparent();
	//描画処理
	m_hpUI_A.Update();
	m_hpUI_B.Update();
	m_hpUI_C.Update();
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
		Vector3 changeS_B = m_scale_B;
		changeS_B.x = m_scale_B.x * ratio;
		m_hpUI_B.SetScale(changeS_B);
		//過去HPに現在のHPを保存
		m_oldhp = m_nowhp;
		//透過遅延開始
		DecreaseTransitionIN1;
	}
}

void HpUi::AdjustmentTransparent()
{
	//スタンバイ状態で処理を実行しない
	if (DecreaseTransition0)
		return;

	//
	if (DecreaseTransition3 || DecreaseTransition4)
	{
		if (DecreaseTransition3)
		{
			float a = m_nowhp;
			float b = m_fullhp;
			//現在HPと最大HPの割合を計算
			float ratio = a / b;
			//現在のm_hpUI_Bのscale.xと割合を計算
			m_changeS_C.x = m_scale_C.x * ratio;
			//減少処理に移行
			DecreaseTransitionIN4;
		}

		//減少処理、繰り返し処理で計算した割合と現在のScale.xと同値になるまで減算
		if (DecreaseTransition4)
		{
			//めちゃくちゃ細かく減らす
			m_nowScale.x -= 0.009;
			//0以下だったら0に戻す
			if (m_nowScale.x < 0)
			{
				m_nowScale.x = 0;
			}
			//大きさを変更する
			m_hpUI_C.SetScale(m_nowScale);

			//現在scaleが変更後scale以下になったら
			if (m_changeS_C.x > m_nowScale.x)
			{
				//念のため初期化
				m_changeS_C = m_scale_C;
				m_nowScale = m_hpUI_C.GetScale();
				//スタンバイに移行
				DecreaseTransitionIN0;
			}
			//変更後以上の場合
			else
				return;
		}
	}

	//ここから下の処理は減少遅延のタイマー処理

	//delaytimer減少処理
	if (0.0 < m_delaytimer && DecreaseTransition2)
	{
		m_delaytimer -= g_gameTime->GetFrameDeltaTime();
	}
	//delaytimerが0になったら
	else if (m_delaytimer < 0.0 && DecreaseTransition2)
	{
		//透過UI減算割合計算処理に移行
		DecreaseTransitionIN3;
		return;
	}

	//m_delayloopがtrueの時はここから後は処理しない
	if (DecreaseTransition2)
		return;

	//透過減少遅延タイマー初期化
	m_delaytimer = m_delaytime;
	//遅延処理に移行
	DecreaseTransitionIN2;
}

void HpUi::Render(RenderContext& rc)
{
	//HPUIの描画処理を先に
	m_hpUI_A.Draw(rc);
	//バーの描画処理を後に
	m_hpUI_B.Draw(rc);
	//透過バーの描画処理を一番後に
	m_hpUI_C.Draw(rc);
}