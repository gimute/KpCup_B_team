#include "stdafx.h"
#include "Enemy.h"
#include "Game.h"
//#include "collision/CollisionObject.h"
#include "Player.h"

#define enemyspeed 150.0f                               //移動速度の数値
#define enemyattackspeed 300.0f                         //攻撃時移動速度の数値
#define enemyserch 500.0f * 500.0f						//追跡可能範囲
#define enemyattack 300.0f * 300.0f						//攻撃可能範囲

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//位置修正
	const Vector3 corre2 = { 2.0f,2.0f,2.0f };
}

Enemy::~Enemy()
{
	m_game->Delete_EnemyVec(m_Vectornum);
}

bool Enemy::Start()
{

	//アニメーション読み込み
	m_animationclips[enAnimationClip_Idle].Load("Assets/modelData/player/proto_player/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Chase].Load("Assets/modelData/player/proto_player/run.tka");
	m_animationclips[enAnimationClip_Chase].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Attack].Load("Assets/modelData/player/proto_player/gunshot.tka");
	m_animationclips[enAnimationClip_Attack].SetLoopFlag(true);//
	//m_animationclips[enAnimationClip_ReceiveDamage].Load("Assets/animData/Enemy/enemy_002/receivedamage.tka");
	//m_animationclips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	//m_animationclips[enAnimationClip_Down].Load("Assets/animData/Enemy/enemy_002/down.tka");
	//m_animationclips[enAnimationClip_Down].SetLoopFlag(false);


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
	//m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	//m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 60.0f * m_scale.z);
	//m_collisionObject->SetName("enemy_col");
	//m_collisionObject->SetPosition(m_position + corre1);
	////コリジョンオブジェクトが自動で削除されないようにする。
	//m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");
	m_Vectornum = m_game->m_EnemyQua;
	m_game->m_EnemyQua++;
	m_game->m_EnemyList.push_back(this);

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	

	return true;
}

void Enemy::Update()
{
	//追跡処理
	Chase();
	//回転処理
	Rotation();
	//攻撃処理
	//Attack();//
	//当たり判定処理
	//Collision();
	//アニメーション
	PlayAnimation();
	//ステート遷移処理
	ManageState();
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
		break;//
	//case enEnemyState_ReceiveDamage:
	//	m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
	//	break;
	//case enEnemyState_Down:
	//	m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
	//	break;

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
		break;//
	//case enEnemyState_ReceiveDamage:
	//	ProcessReceiveDamageStateTransition();
	//	break;
	//case enEnemyState_Down:
	//	ProcessDownStateTransition();
	//	break;
	}
}




void Enemy::ProcessChaseStateTransition()
{
	//射程圏内に入ったら
	if (SearchAttackDistance() == true)
	{
		//他のステートに遷移する。
		m_attackTimer -= g_gameTime->GetFrameDeltaTime();
		ProcessCommonStateTransition();
		return;
	}
	ProcessCommonStateTransition();
}

void Enemy::ProcessAttackStateTransition()
{
	if (m_attackGotimer >= 0) {
		ProcessCommonStateTransition();
	}
	else
	{
		m_enemystate = enEnemyState_Idle;
		//DeleteGO(m_attackcoll);
		m_attackcooltimer = attackcooltime;
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void Enemy::ProcessReceiveDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	//if (m_modelRender.IsPlayingAnimation() == false)
	//{
	//	ProcessCommonStateTransition();
	//}
}

void Enemy::ProcessDownStateTransition()
{
	////被ダメージアニメーションの再生が終わったら。
	//if (m_modelRender.IsPlayingAnimation() == false)
	//{
	//	SoundSource* se = NewGO<SoundSource>(11);
	//	se = NewGO<SoundSource>(11);
	//	se->Init(11);
	//	se->Play(false);
	//	ItemDrop();
	//	DeleteGoEnemyList();
	//	DeleteGO(m_collisionObject);//消去処理
	//	DeleteGO(this);
	//}
}

void Enemy::Chase()
{
	////追跡ステートでないなら、追跡処理はしない。
	//if (IsAttack()) {
	//	return;
	//}
	//if (m_attackGotimer > 0) {
	//	m_attackGotimer -= g_gameTime->GetFrameDeltaTime();
	//}
	//エネミーを移動させる。
	if (m_enemystate == enEnemyState_Idle)
		return;

	m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		//重力を0にする。
		m_movespeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//m_collisionObject->SetRotation(m_rotation);
	//m_collisionObject->SetPosition(m_position + corre1);
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	//座標を設定する。
	m_modelRender.SetPosition(modelPosition);
}

void Enemy::Rotation()
{
	//if (m_enemystate == enEnemyState_Attack) {
	//	return;
	//}
	if (fabsf(m_movespeed.x) < 0.001f
		&& fabsf(m_movespeed.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_movespeed.x, m_movespeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Enemy::Attack()
{
	if (m_enemystate == enEnemyState_Attack) {
		return;
//		//攻撃当たり判定用のコリジョンオブジェクトを作成する。
//		m_attackcoll = NewGO<CollisionObject>(0);
//		//球状のコリジョンを作成する。
//		m_attackcoll->CreateSphere(m_position, Quaternion::Identity, 50.0f * m_scale.z);
//		m_attackcoll->SetName("enemy_attack");
//		Vector3 position = m_position + m_forward * 40.0f;
//		m_attackcoll->SetPosition(position + corre1);
	}
}

//void Enemy::Collision()
//{
//	//被ダメージ、あるいはダウンステートの時は。
////当たり判定処理はしない。
//	if (m_enemystate == enEnemyState_ReceiveDamage ||
//		m_enemystate == enEnemyState_Down)
//	{
//		return;
//	}
//	//無敵時間中は処理しない
//	if (m_mutekitimer > 0)
//	{
//		m_mutekitimer -= g_gameTime->GetFrameDeltaTime();
//		return;
//	}
//	//プレイヤーの攻撃用のコリジョンを取得する。
//	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
//	//コリジョンの配列をfor文で回す。
//	for (auto collision : collisions)
//	{
//		//コリジョンとキャラコンが衝突したら。
//		if (collision->IsHit(m_collisionObject))
//		{
//			SoundSource* se = NewGO<SoundSource>(5);
//			se = NewGO<SoundSource>(5);
//			se->Init(5);
//			se->Play(false);
//			if (m_sh > 0) {
//				m_sh--;
//				//被ダメージステートに遷移する。
//				m_enemystate = enEnemyState_ReceiveDamage;
//				return;
//			}
//			else {
//				//HPを1減らす。
//				m_hp -= 1;
//				m_mutekitimer = mutekitime;
//				//HPが0になったら。
//				if (m_hp == 0) {
//					//ダウンステートに遷移する。
//					m_enemystate = enEnemyState_Down;
//				}
//				else {
//					//被ダメージステートに遷移する。
//					m_enemystate = enEnemyState_ReceiveDamage;
//				}
//			}
//		}
//	}
//}


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
		//角度(θ)が120°(視野角)より小さければ。
		if (angle <= (Math::PI / 180.0f) * 120.0f)
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
	return false;
}

const bool Enemy::SearchAttackDistance() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//プレイヤーにある程度近かったら.。

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
	//各タイマーを初期化。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;
	//エネミーからプレイヤーに向かうベクトルを計算する。

	if (m_attackcooltimer > 0) {
		m_attackcooltimer -= g_gameTime->GetFrameDeltaTime();
		return;
	}
	//if (m_enemystate == enEnemyState_Attack) {
	//	return;
	//}
	Vector3 diff = m_player->GetPosition() - m_position;

	//ベクトルを正規化する。
	//diff.Normalize();
	//移動速度を設定する。
	//m_movespeed = diff * enemyspeed;
	//プレイヤーを見つけたら
	if (SearchPlayer() == true)
	{
		//ベクトルを正規化する。
		diff.Normalize();
		//移動速度を設定する。
		m_movespeed = diff * enemyspeed;

		//攻撃出来る距離かどうか
		if (SearchAttackDistance() == true) {
			m_enemystate = enEnemyState_Attack;

			Rotation();
			m_movespeed = diff * 0.0f;

			return;
		}
		////追跡できる距離かどうか
		//if (SearchChaseDistance() == true) {
		//	if(m_chaseTimer<0.0f)
		//}
		/*if (SearchAttackDistance()) {
			if (m_attackTimer < 0.0f) {
				m_enemystate = enEnemyState_Attack;
				m_attackTimer = attacktime;
				m_attackGotimer = attackGotime;
				m_movespeed = diff * enemyattackspeed;
				return;
			}
			if (m_enemystate == enEnemyState_Attack) {
				m_enemystate = enEnemyState_Chase;
				return;
			}*/
		m_enemystate = enEnemyState_Chase;
		return;
	}
	else
	{
		m_enemystate = enEnemyState_Idle;
		return;
	}
}

void Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

	
}