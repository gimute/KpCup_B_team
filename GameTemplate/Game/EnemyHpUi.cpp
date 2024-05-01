#include "stdafx.h"
#include "EnemyHpUi.h"
#include "Game.h"
#include "Enemy.h"

namespace {
	/// <summary>
	/// //delaytimerの初期設定時間
	/// </summary>
	float m_delaytime = 0.7f;
	/// <summary>
	/// BとCの基点
	/// </summary>
	const Vector2 n_pivot_BC{ 0,0.5 };
}

bool EnemyHpUi::Start()
{
	//画像設定
	m_hpUI_A.Init("Assets/modelData/ui_hp/Enemy_UI_A.DDS", 200.0f, 35.0f);
	m_hpUI_B.Init("Assets/modelData/ui_hp/Enemy_UI_B.DDS", 200.0f, 30.0f);
	m_hpUI_C.Init("Assets/modelData/ui_hp/Enemy_UI_C.DDS", 200.0f, 30.0f);
	//基点設定
	m_hpUI_B.SetPivot(n_pivot_BC);
	m_hpUI_C.SetPivot(n_pivot_BC);

	//ゲームのインスタンスを持ってきて
	m_game = FindGO<Game>("game");

	//HPを初期化
	SetEnemyHp();

	return true;
}

void EnemyHpUi::Update()
{
	//位置更新処理
	PositionUpdate();
	//減算計算処理
	Adjustment();
	//透過減算計算処理
	AdjustmentTransparent();
	//描画処理
	m_hpUI_A.Update();
	m_hpUI_B.Update();
	m_hpUI_C.Update();
}

void EnemyHpUi::PositionUpdate()
{
	Vector3 position = m_game->m_EnemyList[m_Vectornum]->m_position;
	//オブジェクトの上の方に画像を表示したいので。
	//y座標を少し大きくする。
	position.y += 100.0f;
	//ワールド座標からスクリーン座標を計算。
	//計算結果がm_positionAに代入される。
	g_camera3D->CalcScreenPositionFromWorldPosition(m_positionA, position);
	m_hpUI_A.SetPosition(Vector3(m_positionA.x, m_positionA.y, 0.0f));
	//BとCにも位置設定
	m_hpUI_B.SetPosition(Vector3(m_positionA.x - 100.0f, m_positionA.y, 0.0f));
	m_hpUI_C.SetPosition(Vector3(m_positionA.x - 100.0f, m_positionA.y, 0.0f));
}

void EnemyHpUi::Adjustment()
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
		m_decrease_TRAN = en_DelayTimeON_TRAN;
	}
}

void EnemyHpUi::AdjustmentTransparent()
{
	//スタンバイ状態で処理を実行しない
	if (m_decrease_TRAN == en_Standby_TRAN)
		return;

	//
	if (m_decrease_TRAN == en_TransparentDecreaseCalc_TRAN 
		|| m_decrease_TRAN == en_TransparentDecreaseON_TRAN)
	{
		if (m_decrease_TRAN == en_TransparentDecreaseCalc_TRAN)
		{
			float a = m_nowhp;
			float b = m_fullhp;
			//現在HPと最大HPの割合を計算
			float ratio = a / b;
			//現在のm_hpUI_Bのscale.xと割合を計算
			m_changeS_C.x = m_scale_C.x * ratio;
			//減少処理に移行
			m_decrease_TRAN = en_TransparentDecreaseON_TRAN;
		}

		//減少処理、繰り返し処理で計算した割合と現在のScale.xと同値になるまで減算
		if (m_decrease_TRAN == en_TransparentDecreaseON_TRAN)
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
				m_decrease_TRAN = en_Standby_TRAN;
				return;
			}
			//変更後以上の場合
			else
				return;
		}
	}

	//ここから下の処理は減少遅延のタイマー処理

	//delaytimer減少処理
	if (0.0 < m_delaytimer 
		&& m_decrease_TRAN == en_DelayTimeLoop_TRAN)
	{
		m_delaytimer -= g_gameTime->GetFrameDeltaTime();
	}
	//delaytimerが0になったら
	else if (m_delaytimer < 0.0 
		&& m_decrease_TRAN == en_DelayTimeLoop_TRAN)
	{
		//透過UI減算割合計算処理に移行
		m_decrease_TRAN = en_TransparentDecreaseCalc_TRAN;
		return;
	}

	//m_delayloopがtrueの時はここから後は処理しない
	if (m_decrease_TRAN == en_DelayTimeLoop_TRAN)
		return;

	//透過減少遅延タイマー初期化
	m_delaytimer = m_delaytime;
	//遅延処理に移行
	m_decrease_TRAN = en_DelayTimeLoop_TRAN;
}

void EnemyHpUi::Render(RenderContext& rc)
{
	//HPUIの描画処理を先に
	m_hpUI_A.Draw(rc);
	//バーの描画処理を後に
	m_hpUI_B.Draw(rc);
	//透過バーの描画処理を一番後に
	m_hpUI_C.Draw(rc);
}