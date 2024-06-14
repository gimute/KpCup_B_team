#include "stdafx.h"
#include "Enemy.h"
#include "Game.h"
#include "Player.h"
#include "EnemyHpUi.h"
#include "Bullet.h"
#include "EnemyAttackPoint.h"
#include "EnemyCamPos.h"

#define enemyspeed 170.0f                               //移動速度の数値
#define enemyserch 500.0f * 500.0f						//追跡可能範囲
#define enemyattack 300.0f * 300.0f						//攻撃可能範囲

namespace
{
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };//位置修正
	const Vector3 corre2 = { 2.0f,2.0f,2.0f };
}

Enemy::~Enemy()
{
	DeleteGO(m_bgm);
	DeleteGO(m_collisionObject);
	m_game->m_EnemyHpUiList[m_Vectornum]->DeleteUi();
	m_game->Delete_EnemyVec(m_Vectornum);
	ReleaseAttackPoint();
}

bool Enemy::Start()
{
	//テスト
	m_sphereCollider.Create(1.0f);
	//

	//アニメーション読み込み
	m_animationclips[enAnimationClip_Idle].Load("Assets/modelData/player/proto_player/idle.tka");					//待機
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Chase].Load("Assets/modelData/player/proto_player/run.tka");					//走り
	m_animationclips[enAnimationClip_Chase].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Attack].Load("Assets/modelData/player/proto_player/gunshot.tka");				//射撃
	m_animationclips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationclips[enAnimationClip_ShotStandby].Load("Assets/modelData/player/proto_player/shotstandby.tka");		//構え
	m_animationclips[enAnimationClip_ShotStandby].SetLoopFlag(true);
	m_animationclips[enAnimationClip_PostureWalk].Load("Assets/modelData/player/proto_player/gunwalk.tka");			//構え歩き
	m_animationclips[enAnimationClip_PostureWalk].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Damage].Load("Assets/modelData/player/proto_player/receivedamage.tka");		//被ダメージ
	m_animationclips[enAnimationClip_Damage].SetLoopFlag(false);

	//モデル読み込み
	m_modelRender.Init("Assets/modelData/player/proto_player/proto_player2.tkm", m_animationclips, enAnimationClip_Num);

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

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

	//音を読み込む
	//g_soundEngine->ResistWaveFileBank(5, "Assets/sound/m_footSteps.wav");
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/m_atEnemy.wav");
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/m_hpEnemy.wav");

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

	case enEnemyState_Stand:
		Stand();
		break;

	case enEnemyState_ReceiveDamage:

		break;
	}

	//回転処理
	Rotation();
	
	//アニメーション
	PlayAnimation();

	//当たり判定処理
	Collision();
	
	
	if (m_attackTimer > 0.0f && m_enemystate == enEnemyState_Attack)
	{
		m_attackTimer -= g_gameTime->GetFrameDeltaTime();
	}

	//描画更新
	m_modelRender.Update();
}

void Enemy::PlayAnimation()
{
	switch (m_enemystate)
	{
		//待機
	case enEnemyState_Idle:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case enEnemyState_Chase:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Chase, 0.1f);
		break;
	case enEnemyState_Attack:
		if (m_shotBool == true)
		{
			m_modelRender.SetAnimationSpeed(1.0f);
			m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		}
		else if (m_gunmove == true)
		{
			m_modelRender.SetAnimationSpeed(1.0f);
			m_modelRender.PlayAnimation(enAnimationClip_PostureWalk, 0.1f);
		}
		else
		{
			m_modelRender.SetAnimationSpeed(1.0f);
			m_modelRender.PlayAnimation(enAnimationClip_ShotStandby, 0.1f);
		}
		break;
	case enEnemyState_Stand:
		if (m_gunmove == true)
		{
			m_modelRender.SetAnimationSpeed(1.0f);
			m_modelRender.PlayAnimation(enAnimationClip_PostureWalk, 0.1f);
		}
		else
		{
			m_modelRender.SetAnimationSpeed(1.0f);
			m_modelRender.PlayAnimation(enAnimationClip_ShotStandby, 0.1f);
		}
		break;
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(4.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
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
	case enEnemyState_Stand:
		ProcessStandStateTransition();
		break;
	case enEnemyState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
	}
}




void Enemy::ProcessChaseStateTransition()
{
	//エネミーからプレイヤーに向かうベクトルを求める。
	Vector3 diff = m_player->m_position - m_position;

	//プレイヤーとの距離が一定以上なら
	if (diff.Length() >= 700.0f)
	{
		//追跡タイマーを進める
		m_chaseTimer += g_gameTime->GetFrameDeltaTime();

		//タイマーが3秒以上になったら
		if (m_chaseTimer >= 3.0f)
		{
			//状況に応じてステートを遷移
			ProcessCommonStateTransition();
			m_chaseTimer = 0.0f;
			return;
		}
	}
	else
	{
		m_chaseTimer = 0.0f;
	}

	//利用可能なアタックポイントを探す
	EnemyAttackPoint::AttackPoint* attackPoint;

	attackPoint = m_game->GetEnemyAttackPointObject()->GetNearAttackPoint(m_position);

	//アタックポイントが確保できなかった場合
	if (attackPoint == nullptr)
	{
		//プレイヤーとの距離が一定以下なら
		if ((m_player->GetPosition() - m_position).Length() <= 300.0f)
		{
			//ステートを構えに変更

			m_enemystate = enEnemyState_Stand;
			return;
		}
	}
	//確保できた場合
	else
	{
		//アタックポイントとの距離が一定以下なら
		if ((attackPoint->m_position - m_position).Length() <= 50.0f)
		{
			//ステートを攻撃に変更
			m_enemystate = enEnemyState_Attack;
			m_shotBool = false;
			m_attackTimer = m_game->GetEnemyCamPosInstance()->EnemyCamPosConfirmation(this);
		}
	}
}

void Enemy::ProcessAttackStateTransition()
{

	//アタックポイントを確保していなかったら
	if (m_AttackPoint == nullptr)
	{
		//利用可能なアタックポイント取得
		m_AttackPoint = m_game->GetEnemyAttackPointObject()->GetNearAttackPoint(m_position,this);

		//アタックポイントを取得できなかったら
		if (m_AttackPoint == nullptr)
		{
			//追跡ステートにする
			m_enemystate = enEnemyState_Chase;
			return;
		}
	}
	
	//エネミーからアタックポイントに向かうベクトルを求める
	Vector3 diff = m_AttackPoint->m_position - m_position;

	//プレイヤーが一定以上離れたら
	if ((m_player->m_position - m_position).Length() >= 400.0f)
	{
		//アタックポイントを解放して
		ReleaseAttackPoint();
		// ステートを遷移する
		m_enemystate = enEnemyState_Chase;
		//ProcessCommonStateTransition();
	}
}

void Enemy::ProcessIdleStateTransition()
{
	// 待機状態からはいつでも他のステートに遷移可能
	ProcessCommonStateTransition();
}

void Enemy::ProcessStandStateTransition()
{
	//アタックポイントが空いていたら
	if (m_game->GetEnemyAttackPointObject()->IsUsableAttackPoint() == true)
	{
		//追跡ステートにしてアタックポイントを取りに行く
		m_enemystate = enEnemyState_Chase;
	}

	Vector3 diff = m_player->m_position - m_position;
	//プレイヤーとの距離が一定以上離れたら
	if (diff.Length() >= 400.0f)
	{
		//追跡ステートにする
		m_enemystate = enEnemyState_Chase;
	}
}

void Enemy::Chase()
{
	//利用可能なアタックポイントを探す
	EnemyAttackPoint::AttackPoint* attackPoint;
	attackPoint = m_game->GetEnemyAttackPointObject()->GetNearAttackPoint(m_position);

	//アタックポイントが確保できない場合
	if (attackPoint == nullptr)
	{
		//プレイヤーに向かわせる

		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();

		m_movespeed = diff * enemyspeed;

		m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
		m_modelRender.SetPosition(m_position);
	}
	//確保できた場合
	else
	{
		//アタックポイントに向かわせる

		Vector3 diff = attackPoint->m_position - m_position;
		diff.Normalize();

		m_movespeed = diff * enemyspeed;

		m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
		m_modelRender.SetPosition(m_position);
	}
}

void Enemy::ProcessReceiveDamageStateTransition()
{
	//被ダメージモーションが終わったら
	if(m_modelRender.IsPlayingAnimation() == false)
	{
		//元のステートが待機なら
		if (m_oldEnemyState == enEnemyState_Idle)
		{
			//追跡ステートにする
			m_enemystate = enEnemyState_Chase;
		}
		//待機ステート以外なら
		else
		{

			m_shotBool = false;
			//元のステートに戻す
			m_enemystate = m_oldEnemyState;
		}
	}
}

void Enemy::Rotation()
{
	// プレイヤーに向かうベクトルを求める。
	Vector3 diff = m_player->GetPosition() - m_position;

	// ステートによってどこに向かせるか変える
	switch (m_enemystate)
	{
	case Enemy::enEnemyState_Idle:
		break;

		
	case Enemy::enEnemyState_Chase:
		//進行方向に向かせる
		if (fabsf(m_movespeed.x) < 0.001f && fabsf(m_movespeed.z) < 0.001f) {
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

	case Enemy::enEnemyState_Attack:
		//プレイヤーのいる方向に向かせる
		//モデルの正面方向(z軸方向に伸びる単位ベクトル)から、プレイヤーに向かうベクトル方向に回転させるクオータニオンを作成。
		m_rotation.SetRotation(Vector3::AxisZ, diff);
		//作成したクオータニオンをモデルのローテーションに適応。
		m_modelRender.SetRotation(m_rotation);

		//エネミーの正面方向ベクトルを求める
		m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);
		break;

	case Enemy::enEnemyState_Stand:
		//プレイヤーのいる方向に向かせる
		//モデルの正面方向(z軸方向に伸びる単位ベクトル)から、プレイヤーに向かうベクトル方向に回転させるクオータニオンを作成。
		m_rotation.SetRotation(Vector3::AxisZ, diff);
		//作成したクオータニオンをモデルのローテーションに適応。
		m_modelRender.SetRotation(m_rotation);

		//エネミーの正面方向ベクトルを求める
		m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);
		break;
	}
}

void Enemy::Attack()
{
	if (m_attackTimer <= 0.0f)
	{
		m_shotBool = true;
	}
	else
	{
		m_enemyAttackStep = en_stanceStep;
	}

	//アタックポイントが確保で来ていたら
	if (m_AttackPoint != nullptr)
	{
		//アタックポイントまでのベクトルを求める
		Vector3 diff = m_AttackPoint->m_position - m_position;

		//アタックポイントとの距離が一定以上かつ、射撃フラグがfalseなら
		if (diff.Length() >= 80.0f && m_shotBool == false)
		{
			m_AttackPoint = m_game->GetEnemyAttackPointObject()->ReGetNearAttackPoint(this, m_AttackPoint);

			diff = m_AttackPoint->m_position - m_position;
			//アタックポイントに向かって移動
			diff.Normalize();

			//銃を構えながらの移動なので少し移動速度を落とす
			m_movespeed = diff * enemyspeed * 0.7f;

			m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
			m_modelRender.SetPosition(m_position);

			//モーションを構え歩きにするためのフラグをtrueにする
			m_gunmove = true;
		}
		else
		{
			m_gunmove = false;
		}
	}
}

void Enemy::Stand()
{
	//エネミーからプレイヤーに向かうベクトル
	Vector3 diff = m_player->m_position - m_position;

	//プレイヤーとの距離が近い時
	if (diff.Length() <= 100.0f)
	{
		m_movespeed = m_forward * -1 * enemyspeed * 0.7f;

		m_position = m_charaCon.Execute(m_movespeed, g_gameTime->GetFrameDeltaTime());
		m_modelRender.SetPosition(m_position);

		//モーションを構え歩きにするためのフラグをtrueにする
		m_gunmove = true;
	}
	else
	{
		m_gunmove = false;
	}
}

void Enemy::Collision()
{
	//被ダメージステートの時は当たり判定処理をしない
	if (m_enemystate == enEnemyState_ReceiveDamage)
	{
		return;
	}

	Vector3 tmp = m_position;
	tmp.y += 30.0f;
	m_collisionObject->SetPosition(tmp);

	//プレイヤーの攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_collisionObject))
		{	
				//HPを1減らす。
				m_hp -= 1;
				//効果音を再生する
				SoundSource* m_hpPlayer = NewGO<SoundSource>(0);
				m_hpPlayer = NewGO<SoundSource>(0);
				m_hpPlayer->Init(10);
				m_hpPlayer->Play(false);

				m_game->m_EnemyHpUiList[m_Vectornum]->DecreaseHP(1);
				//m_mutekitimer = mutekitime;
				//HPが0になったら。
				if (m_hp == 0) {
					DeleteGO(this);
				}
				else {
					//被ダメージ前のステートを記憶
					m_oldEnemyState = m_enemystate;

					//被ダメージステートに遷移する。
					m_enemystate = enEnemyState_ReceiveDamage;
				}

				//
				AroundEnemyTransitionToChase();
		}
	}
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

bool Enemy::WallCheck(const Vector3 position)
{
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標
	start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
	//終点は引数で受け取った座標の座標
	end.setOrigin(btVector3(position.x, position.y + 70.0f, position.z));

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
///////////////////////////////////////////////////////////////////////

bool Enemy::SearchPlayer()
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
			//プレイヤーとエネミーの間に壁があるか調べる
			if (WallCheck(m_player->GetPosition()) == false)
			{
				//プレイヤーを見つけた！
				return true;
			}	
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
	//プレイヤーが視界内に居るか
	if (SearchPlayer())
	{
		//周囲の
		AroundEnemyTransitionToChase();

		//追跡ステートに
		m_enemystate = enEnemyState_Chase;
	}
	else
	{
		//居なければ待機ステートに
		m_enemystate = enEnemyState_Idle;
	}
}

void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"B_signal_start") == 0)
	{
		m_enemyAttackStep = en_shotStep;
	}
	else if (wcscmp(eventName, L"A_shot_point") == 0)
	{
		Vector3 m_shotPos = m_player->GetPosition() - m_position;
		m_shotPos.Normalize();

		m_bullet = NewGO<Bullet>(0, "bullet");
		Quaternion rot;
		rot.SetRotation(Vector3::AxisZ,m_shotPos);
		m_bullet->SetMoveDirection(m_forward);
		m_bullet->Setrotation(rot);
		m_bullet->SetPosition(m_position);
		m_bullet->SetShotType(Bullet::en_Enemy);

		//効果音を再生する
		SoundSource* m_atEnemy = NewGO<SoundSource>(0);
		m_atEnemy->Init(9);
		m_atEnemy->Play(false);
		m_atEnemy->SetVolume(2.0f);
	}
	else if (wcscmp(eventName, L"C_shot_end") == 0)
	{
		m_attackTimer = m_game->GetEnemyCamPosInstance()->EnemyCamPosConfirmation(this);
		m_enemyAttackStep = en_noneStep;
		m_shotBool = false;
	}
}

void Enemy::ReleaseAttackPoint()
{
	//アタックポイントを持っていなければ何もせず返す
	if (m_AttackPoint == nullptr)
	{
		return;
	}

	//アタックポイントを解放
	m_game->GetEnemyAttackPointObject()->ReleaseAttackPoint(m_AttackPoint->m_number, this);
	//アタックポイントへのポインタも解放
	m_AttackPoint = nullptr;
}

void Enemy::AroundEnemyTransitionToChase()
{
	for (int i = 0; i < m_game->m_EnemyList.size(); i++)
	{
		m_game->m_EnemyList[i]->TransitionToChaseTest(this);
	}
}

void Enemy::TransitionToChaseTest(Enemy* enemy)
{
	//ステートがidle
	if (m_enemystate == enEnemyState_Idle)
	{
		Vector3 diff = m_position - enemy->m_position;
		//伝播元との距離が一定以上かつ、伝播元が自分ではない
		if (diff.Length() <= 600.0f && this != enemy)
		{
			//伝播元と自分の間に壁がない
			if (WallCheck(enemy->m_position) == false)
			{
				//ステートをchaseに
				m_enemystate = enEnemyState_Chase;
			}
		}
	}
}

void Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}