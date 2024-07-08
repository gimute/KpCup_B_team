#include "stdafx.h"
#include "EnemyHpUi.h"
#include "Game.h"
#include "Enemy.h"
#include "Player.h"

namespace {
	/// <summary>
	/// delaytimerの初期設定時間
	/// </summary>
	float m_delaytime = 0.7f;
	/// <summary>
	/// BとCの基点
	/// </summary>
	const Vector2 n_pivot_BC{ 0,0.5 };
	/// <summary>
	/// 表示限界距離
	/// </summary>
	const float LimitedRange = 600.0f;
}

bool EnemyHpUi::Start()
{
	//画像設定
	m_hpUI_A.Init("Assets/modelData/ui_hp/Enemy_UIV2_A.DDS", 100.0f, 15.0f);
	m_hpUI_B.Init("Assets/modelData/ui_hp/Enemy_UIV2_B.DDS", 100.0f, 10.0f);
	m_hpUI_C.Init("Assets/modelData/ui_hp/Enemy_UIV2_C.DDS", 100.0f, 10.0f);
	//基点設定
	m_hpUI_B.SetPivot(n_pivot_BC);
	m_hpUI_C.SetPivot(n_pivot_BC);

	//ゲームのインスタンスを持ってきて
	m_game = FindGO<Game>("game");
	//プレイヤーのインスタンスも持ってくる
	m_player = FindGO<Player>("player");

	//HPを初期化
	SetEnemyHp();

	m_sphereCollider.Create(0.5f);

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
	//表示距離計算処理
	DisplayDis();
	//描画処理
	m_hpUI_A.Update();
	m_hpUI_B.Update();
	m_hpUI_C.Update();
}

void EnemyHpUi::DeleteUi()
{
	DeleteGO(this);
}

//WallCheckに使っている構造体、WallCheckの位置を動かすときは一緒に動かしてください
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& covexResult, bool normalInWorldSpace)
	{
		//壁とぶつかっていなかったら
		if (covexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall)
		{
			//衝突したのは壁ではない
			return 0.0f;
		}

		//壁とぶつかったらフラグをtrueにする
		isHit = true;
		return 0.0f;
	}
};

void EnemyHpUi::DisplayDis()
{
	//プレイヤーの位置を取得
	Vector3 DisplayCenterPos = m_player->GetPosition();
	//自身の配列番号から一致するエネミーの位置を取得
	Vector3 DisplayTargetPos = m_game->GetEnemyListPos(m_Vectornum);

	//取得したプレイヤーの位置から取得したエネミーの位置まで伸びるベクトルを計算
	Vector3 diff = DisplayTargetPos - DisplayCenterPos;


	if (AngleCheck(DisplayCenterPos, DisplayTargetPos) || diff.LengthSq() >= LimitedRange * LimitedRange)
	{
		//表示しないようにする。
		m_isImage = false;
	}
	else
	{
		//表示する
		m_isImage = true;
	}
}

bool EnemyHpUi::AngleCheck(const Vector3 PlayerPosition, const Vector3 EnemyPosition)
{
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はプレイヤーの座標
	start.setOrigin(btVector3(PlayerPosition.x, PlayerPosition.y + 70.0f, PlayerPosition.z));
	//終点はエネミーの座標
	end.setOrigin(btVector3(EnemyPosition.x, EnemyPosition.y + 70.0f, EnemyPosition.z));

	SweepResultWall callback;
	//制作したコライダーを始点から終点まで動かして壁に接触したか判定
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//壁と衝突した時
	if (callback.isHit == true)
	{
		return true;
	}

	return false;
}

void EnemyHpUi::PositionUpdate()
{
	Vector3 position = m_game->GetEnemyListPos(m_Vectornum);
	//オブジェクトの上の方に画像を表示したいので。
	//y座標を少し大きくする。
	position.y += 80.0f;
	//ワールド座標からスクリーン座標を計算。
	//計算結果がm_positionAに代入される。
	g_camera3D->CalcScreenPositionFromWorldPosition(m_positionA, position);
	m_hpUI_A.SetPosition(Vector3(m_positionA.x, m_positionA.y, 0.0f));
	//BとCにも位置設定
	m_hpUI_B.SetPosition(Vector3(m_positionA.x - 50.0f, m_positionA.y, 0.0f));
	m_hpUI_C.SetPosition(Vector3(m_positionA.x - 50.0f, m_positionA.y, 0.0f));
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

	//透過UI減少計算中または透過減少中であれば
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
	if (m_isImage)
	{
		//HPUIの描画処理を先に
		m_hpUI_A.Draw(rc);
		//バーの描画処理を後に
		m_hpUI_C.Draw(rc);
		//透過バーの描画処理を一番後に
		m_hpUI_B.Draw(rc);
	}
}