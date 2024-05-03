#include "stdafx.h"
#include "Enemy.h"
#include "Game.h"
#include "Player.h"
#include "EnemyHpUi.h"

#define enemyspeed 150.0f                               //移動速度の数値
#define enemyserch 500.0f * 500.0f						//追跡可能範囲
#define enemyattack 300.0f * 300.0f						//攻撃可能範囲

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//位置修正
	const Vector3 corre2 = { 2.0f,2.0f,2.0f };
}

Enemy::~Enemy()
{
	DeleteGO(m_collisionObject);
}

bool Enemy::Start()
{

	//アニメーション読み込み
	m_animationclips[enAnimationClip_Idle].Load("Assets/modelData/player/proto_player/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Chase].Load("Assets/modelData/player/proto_player/run.tka");
	m_animationclips[enAnimationClip_Chase].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Attack].Load("Assets/modelData/player/proto_player/gunshot.tka");
	m_animationclips[enAnimationClip_Attack].SetLoopFlag(true);
	m_animationclips[enAnimationClip_ShotStandby].Load("Assets/modelData/player/proto_player/shotstandby.tka");
	m_animationclips[enAnimationClip_Attack].SetLoopFlag(true);

	//モデル読み込み
	m_modelRender.Init("Assets/modelData/player/proto_player/proto_player2.tkm", m_animationclips, enAnimationClip_Num);

	//回転
	m_modelRender.SetRotation(m_rotation);
	//座標
	m_modelRender.SetPosition(m_position);
	//スケール
	m_modelRender.SetScale(m_scale);
	//キャラコン初期化
	m_charaCon.Init(20.0f, 0.0f, m_position);

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 30.0f * m_scale.z);
	m_collisionObject->SetName("enemy_col");
	m_collisionObject->SetPosition(m_position + corre1);
	////コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");
	m_Vectornum = m_game->m_EnemyQua;
	m_game->m_EnemyQua++;
	m_game->m_EnemyList.push_back(this);

	//HPUIを作成
	EnemyHpUi* m_enemyHpUi = NewGO<EnemyHpUi>(0, "Ui");
	m_enemyHpUi->m_Vectornum = m_Vectornum;
	m_game->m_EnemyHpUiList.push_back(m_enemyHpUi);

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	

	return true;
}

void Enemy::Update()
{
	//ステート遷移処理
	ManageState();
	
	//ステートごとの専用の処理
	switch (m_enemystate)
	{
	case enEnemyState_Idle:

		break;

	case enEnemyState_Chase:
		//追跡処理
		Chase();
		break;

	case enEnemyState_Attack:
		//攻撃処理
		Attack();
		break;
	}

	//回転処理
	Rotation();
	
	//当たり判定処理
	Collision();
	//アニメーション
	PlayAnimation();
	
	
	//描画更新
	m_modelRender.Update();
}

void Enemy::PlayAnimation()
{
	//m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_enemystate)
	{
		//待機
	case enEnemyState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case enEnemyState_Chase:
		m_modelRender.PlayAnimation(enAnimationClip_Chase, 0.1f);
		break;
	case enEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	}
}

void Enemy::ManageState()
{
	switch (m_enemystate)
	{
	case enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	}
}




void Enemy::ProcessChaseStateTransition()
{
	// アタックポイントを確保できている場合、
	if (HaveAttackPoint() == true)
	{
		// アタックポイントとの距離が一定以下、もしくはプレイヤーとの距離が一定以下なら
		if ((m_enemeAttackPoint->m_position - m_position).Length() <= 10.0f || (m_player->m_position - m_position).Length() <= 200.0f)
		{
			// 直接アタックステートにする。
			m_enemystate = enEnemyState_Attack;
			m_chaseTimer = 3.0f;
			return;
		}
		else
		{
			if (SearchChaseDistance())
			{
				m_chaseTimer = 3.0f;
			}
			else
			{
				m_chaseTimer -= g_gameTime->GetFrameDeltaTime();
			}

			if (m_chaseTimer <= 0)
			{
				ProcessCommonStateTransition();
			}
			return;
		}
	}
	// アタックポイントを確保できていない場合、
	else
	{
		// プレイヤーがまだ近くにいる時、
		if (SearchChaseDistance())
		{
			// プレイヤーが攻撃範囲内に居たら、
			if (SearchAttackDistance() == true)
			{
				// ステートを遷移する。
				ProcessCommonStateTransition();
				m_chaseTimer = 3.0f;
				return;
			}
			// 攻撃範囲内で無ければ
			else
			{
				// 追跡ステートのまま返す。
				m_chaseTimer = 3.0f;
				return;
			}	
		}
		// プレイヤーが近くに居ない場合
		else
		{
			//チェイスタイマーを進める(値を減らす)
			m_chaseTimer -= g_gameTime->GetFrameDeltaTime();
			//タイマーが0以下なら
			if (m_chaseTimer <= 0)
			{
				// ステートを遷移する
				ProcessCommonStateTransition();
			}
			return;
		}
	}
}

void Enemy::ProcessAttackStateTransition()
{
	// 今のところはいつでも他のステートに遷移可能
	// 後でモーションが終了するまで等の条件を追加
	ProcessCommonStateTransition();
}

void Enemy::ProcessIdleStateTransition()
{
	// 待機状態からはいつでも他のステートに遷移可能
	ProcessCommonStateTransition();
}

void Enemy::Chase()
{
	//念のためステート確認
	if (m_enemystate != enEnemyState_Chase)
	{
		return;
	}

	//アタックポイントを確保
	GetAttackPoint();

	//アックポイントが確保できなければ
	if (HaveAttackPoint() == false)
	{
		//プレイヤーに直接向かう

		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();

		m_movespeed = diff * enemyspeed;

		m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());

		m_modelRender.SetPosition(m_position);
	}
	//アタックポイントを確保できているなら
	else
	{
		//アタックポイントに向かわせる

		//アタックポイントに向かうベクトルを求める
		Vector3 diff = m_enemeAttackPoint->m_position - m_position;

		//アタックポイントとの距離が一定以上離れていたら、
		if (diff.Length() < 300.0f)
		{
			//一度アタックポイントをリリースし、
			ReleaseAttackPoint();
			//近くにあるポイントを確保しなおす。
			GetAttackPoint();
		}

		diff.Normalize();

		m_movespeed = diff * enemyspeed;

		m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());

		m_modelRender.SetPosition(m_position);
	}
}

void Enemy::Rotation()
{
	// プレイヤーに向かうベクトルを求める。
	Vector3 diff = m_player->GetPosition() - m_position;

	// ステートによって処理を変える
	switch (m_enemystate)
	{
	case Enemy::enEnemyState_Idle:
		break;

		//追跡ステートの場合///////////////////////////////////////////////////////
		//進行方向に向かせる
	case Enemy::enEnemyState_Chase:
		if (fabsf(m_movespeed.x) < 0.001f
			&& fabsf(m_movespeed.z) < 0.001f) {
			//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
			//このフレームではキャラは移動していないので旋回する必要はない。
			return;
		}

		//movespeedからエネミーの向きを求める
		m_rotation.SetRotationY(-atan2(-m_movespeed.x, m_movespeed.z));

		//回転を設定する。
		m_modelRender.SetRotation(m_rotation);

		//エネミーの正面方向ベクトルを求める
		m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);
		break;
		///////////////////////////////////////////////////////////////////////////

		//アタックステートの場合///////////////////////////////////////////////////
		//プレイヤーのいる方向に向かせる
	case Enemy::enEnemyState_Attack:
		//モデルの正面方向(z軸方向に伸びる単位ベクトル)から、プレイヤーに向かうベクトル方向に回転させるクオータニオンを作成。
		m_rotation.SetRotation(Vector3::AxisZ, diff);
		//作成したクオータニオンをモデルのローテーションに適応。
		m_modelRender.SetRotation(m_rotation);

		//エネミーの正面方向ベクトルを求める
		m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);
		break;
		//////////////////////////////////////////////////////////////////////////

	default:
		break;
	}
}

void Enemy::Attack()
{
	//念のためステート確認
	if (m_enemystate != enEnemyState_Attack)
	{
		return;
	}

	//攻撃処理
}

void Enemy::Collision()
{
	Vector3 tmp = m_position;
	tmp.y += 30.0f;
	m_collisionObject->SetPosition(tmp);
	//被ダメージ、あるいはダウンステートの時は。
//当たり判定処理はしない。
	/*if (m_enemystate == enEnemyState_ReceiveDamage ||
		m_enemystate == enEnemyState_Down)
	{
		return;
	}*/
	//無敵時間中は処理しない
	/*if (m_mutekitimer > 0)
	{
		m_mutekitimer -= g_gameTime->GetFrameDeltaTime();
		return;
	}*/
	//プレイヤーの攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_collisionObject))
		{
			//SoundSource* se = NewGO<SoundSource>(5);
			//se = NewGO<SoundSource>(5);
			//se->Init(5);
			//se->Play(false);
			//if (m_sh > 0) {
			//	m_sh--;
			//	//被ダメージステートに遷移する。
			//	m_enemystate = enEnemyState_ReceiveDamage;
			//	return;
			//}
			
				//HPを1減らす。
				m_hp -= 1;
				m_game->m_EnemyHpUiList[m_Vectornum]->DecreaseHP(1);
				//m_mutekitimer = mutekitime;
				//HPが0になったら。
				if (m_hp == 0) {
					//ダウンステートに遷移する。
					//m_enemystate = enEnemyState_Idle;
					m_game->m_EnemyHpUiList[m_Vectornum]->DeleteUi();
					m_game->Delete_EnemyVec(m_Vectornum);
					DeleteGO(this);
				}
				else {
					//被ダメージステートに遷移する。
					m_enemystate = enEnemyState_Idle; //被ダメージステートが無いので、仮として待機ステート置いている
				}
			
		}
	}
}


const bool Enemy::SearchPlayer() const
{
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーにある程度近かったら.。
	if (diff.LengthSq() <= enemyserch)
	{
		// 敵からプレイヤーに向かうベクトルを正規化する
		diff.Normalize();
		//敵の正面のベクトルと、敵からプレイヤーに向かうベクトルの。
		//内積(cosθ)を求める。
		float cos = m_forward.Dot(diff);
		if (cos >= 1)
		{
			cos = 1.0f;
		}
		//内積(cosθ)から角度(θ)を求める。
		float angle = acosf(cos);
		//角度(θ)が90°(視野角)より小さければ。
		if (angle <= (Math::PI / 180.0f) * 90.0f)
		{
			//プレイヤーを見つけた！
			return true;
		}
	}
	//プレイヤーを見つけられなかった。
	return false;
}
const bool Enemy::SearchChaseDistance() const
{
	//プレイヤーに向かうベクトルを計算
	Vector3 diff = m_player->GetPosition() - m_position;
	//求めたベクトルの距離が一定より短ければ
	if (diff.Length() <= 500.0f)
	{
		//プレイヤーが近くにいる
		return true;
	}
	//プレイヤーが近くにいない
	return false;
}

const bool Enemy::SearchAttackDistance() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//プレイヤーにある程度近かったら。

	if (diff.LengthSq() <= enemyattack)
	{
		//プレイヤーが射程圏内に入った！
		return true;
	}
	//プレイヤーが射程圏外だった。
	return false;
}

void Enemy::ProcessCommonStateTransition()
{
	//プレイヤーが視界内に居るか、
	if (SearchPlayer())
	{
		//攻撃できる距離なら
		if (SearchAttackDistance())
		{
			//ステートをアタックにする。
			m_enemystate = enEnemyState_Attack;
		}
		//攻撃範囲内ではないなら
		else
		{
			//ステートを追跡にする。
			m_enemystate = enEnemyState_Chase;
		}
	}
	//プレイヤーが視界内に居ないなら
	else
	{
		//アタックポイントを解放して
		ReleaseAttackPoint();
		//ステートを待機にする
		m_enemystate = enEnemyState_Idle;
	}
}

const bool Enemy::HaveAttackPoint() const
{
	//アタックポイントを確保できていなかったら
	if (m_enemeAttackPoint == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}

}

void Enemy::GetAttackPoint()
{
	//アタックポイント持っていなければ
	if (m_enemeAttackPoint == nullptr)
	{
		//空いている一番近いアタックポイントを受け取る
		m_enemeAttackPoint = m_game->GetEnemyAttackPoint(m_position);
	}
}

void Enemy::ReleaseAttackPoint()
{
	//そもそもアタックポイントを持っていなかったら何もせず返す。
	if (m_enemeAttackPoint == nullptr)
	{
		return;
	}

	//確保しているアタックポイントを未使用中にし、
	m_enemeAttackPoint->m_use = false;
	//アタックポイントを開放する
	m_enemeAttackPoint = nullptr;
}

void Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

	
}