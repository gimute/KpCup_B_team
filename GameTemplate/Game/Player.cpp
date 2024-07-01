#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "Game.h"
#include "Enemy.h"
#include "HpUi.h"
#include "RemainingBulletsUi.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"


class LanchSlowMotion : public IGameObject
{
	float m_timer = 0;
	Game* m_game;
public:
	void Initialize(Game* game)
	{
		m_game = game;
	}
	void Update()
	{
		m_timer += g_gameTime->GetFrameDeltaTime();
		if (m_timer > 0.05f) {
			m_game->SlowStart(0.05f);
			DeleteGO(this);
		}
	}
};
Player::Player()
{

}

Player::~Player()
{
	DeleteGO(m_collisionObject);
	DeleteGO(m_playerWalk);
	DeleteGO(m_playerStance);
}

bool Player::Start()
{
	m_animationclips[enAnimationClip_Idle].Load("Assets/modelData/player/proto_player/idle.tka");
	m_animationclips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Walk].Load("Assets/modelData/player/proto_player/run.tka");
	m_animationclips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Crouching].Load("Assets/modelData/player/proto_player/crouching.tka");
	m_animationclips[enAnimationClip_Crouching].SetLoopFlag(false);
	m_animationclips[enAnimationClip_Punch].Load("Assets/modelData/player/proto_player/punch.tka");
	m_animationclips[enAnimationClip_Punch].SetLoopFlag(false);
	m_animationclips[enAnimationClip_Gunshot].Load("Assets/modelData/player/proto_player/gunshot_short.tka");
	m_animationclips[enAnimationClip_Gunshot].SetLoopFlag(false);
	m_animationclips[enAnimationClip_PostureWalk].Load("Assets/modelData/player/proto_player/gunwalk.tka");
	m_animationclips[enAnimationClip_PostureWalk].SetLoopFlag(true);
	m_animationclips[enAnimationClip_PostureIdle].Load("Assets/modelData/player/proto_player/shotstandby.tka");
	m_animationclips[enAnimationClip_PostureIdle].SetLoopFlag(true);
	m_animationclips[enAnimationClip_Rolling].Load("Assets/modelData/player/proto_player/rolling.tka");
	m_animationclips[enAnimationClip_Rolling].SetLoopFlag(false);
	m_animationclips[enAnimationClip_Damage].Load("Assets/modelData/player/proto_player/receivedamage.tka");
	m_animationclips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationclips[enAnimationClip_knockdown].Load("Assets/modelData/player/proto_player/knockdown.tka");
	m_animationclips[enAnimationClip_knockdown].SetLoopFlag(false);

	m_modelRender.Init("Assets/modelData/player/proto_player/proto_player2.tkm", m_animationclips, enAnimationClip_Num);

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//スケール
	m_modelRender.SetScale(m_scale);

	m_charaCon.Init(25.0f, 40.0f, m_position);
	
	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 30.0f * m_scale.z);
	m_collisionObject->SetName("player_col");
	m_collisionObject->SetPosition(m_position);
	////コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);

	//コリジョンオブジェクトを作成する。
	m_justRollingCollisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	Vector3 tmp;
	tmp = m_position;
	tmp.y += 30.0f;
	m_justRollingCollisionObject->CreateSphere(tmp, Quaternion::Identity, 45.0f * m_scale.z);
	m_justRollingCollisionObject->SetName("player_justrolling");
	m_justRollingCollisionObject->SetIsEnableAutoDelete(false);

	m_sphereCollider.Create(1.0f);

	m_game = FindGO<Game>("game");

	//音を読み込む
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/m_gunSound.wav");
	g_soundEngine->ResistWaveFileBank(8, "Assets/sound/m_hpPlayer.wav");
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/m_rolling.wav");
	g_soundEngine->ResistWaveFileBank(13,"Assets/sound/m_gunStance.wav");

	//残弾管理Ui登録
	m_remainingBulletsUi = FindGO<RemainingBulletsUi>("UI");
	m_remainingBulletsUi->InitRemainingBulletNum(&m_remainingBullets);

	m_playerWalk = NewGO<SoundSource>(0);
	m_playerWalk->Init(5);

	m_playerStance = NewGO<SoundSource>(0);
	m_playerStance->Init(14);

	//m_gunStance = NewGO<SoundSource>(13);
	return true;
}

void Player::Update()
{
	//ステートがイベントの時
	if (m_playerstate == enPlayerState_Event)
	{
		Vector3 posToTar = m_eventInfos[m_eventInfoNum].m_targetPos - m_position;
		posToTar.y = 0.0f;
		if (posToTar.Length() >= 10.0f)
		{
			posToTar.Normalize();
			posToTar *= 250.0f;
			m_position = m_charaCon.Execute(posToTar, g_gameTime->GetFrameDeltaTime());
			//m_position += posToTar * 200.0f;
		}
		else
		{
			if (m_eventInfos[m_eventInfoNum].m_waitTime <= m_eventTimer)
			{
				if (m_eventInfos[m_eventInfoNum].m_eventState == m_eventInfos[m_eventInfoNum + 1].m_eventState)
				{
					m_eventInfoNum++;
					m_eventTimer = 0.0f;
				}
				else
				{
					m_playerstate = enPlayerState_Idle;
					m_eventTimer = 0.0f;
					return;
				}
			}
			else
			{
				m_eventTimer += g_gameTime->GetFrameDeltaTime();
			}
		}

		if (fabsf(posToTar.x) >= 0.001f || fabsf(posToTar.z) >= 0.001f)
		{
			//キャラクターの方向を変える。
			m_rotation.SetRotationYFromDirectionXZ(posToTar);
			//絵描きさんに回転を教える。
			m_modelRender.SetRotation(m_rotation);
		}

		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(m_eventInfos[m_eventInfoNum].m_motion, 0.1f);
		m_charaCon.SetPosition(m_position);
		m_modelRender.SetPosition(m_position);
		m_modelRender.Update();

		//通常の処理は実行せず返す
		return;
	}

	//移動処理。
	Move();
	//回転処理。
	Rotation();
	//当たり判定処理
	Collision();
	//アニメーション処理
	PlayAnimation();
	//ステートの遷移処理
	ManageState();

	if (m_playerstate == enPlayerState_KnockDown)
	{
		m_gameoverWaitTimer += g_gameTime->GetFrameDeltaTime();
		if (m_gameoverWaitTimer >= 2.0f)
		{
			if (m_game->GetGmaeState() == Game::enIdle)
			{
				m_game->NotifyGameOver();
			}
			
		}
	}

	if (m_justRollingTime > 0.0f)
	{
		m_justRollingTime -= g_gameTime->GetFrameDeltaTime();
	}
	else
	{
		Vector3 tmp;
		tmp = m_position;
		tmp.y += 30.0f;
		m_justRollingCollisionObject->SetPosition(tmp);
		m_justRollingCol = false;
	}

	if (m_rollingCorrectionTime >= 0.1f)
	{
		m_rollingCorrectionTime -= g_gameTime->GetFrameDeltaTime();
		if (g_pad[0]->IsPress(enButtonRB1))
		{
			RollingEndRot();
		}
	}

	if (m_reloadTime > 0.0)
	{
		m_reloadTime -= g_gameTime->GetFrameDeltaTime();
	}
	else if (m_isReload)
	{
		m_remainingBullets = 17;
		m_remainingBulletsUi->IsReload(false);
		m_isReload = false;
	}

	//モデルの更新。
	m_modelRender.Update();

}

void Player::Move()
{
	if (m_game->GetPlayerMove())
	{
		return;
	}

	//ステートが回避の場合
	if (m_playerstate == enPlayerState_Rolling)
	{
		//回避処理に移行する
		Rolling();
		return;
	}


	//xzの移動速度を0.0fにする。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//左スティックの入力量を取得。
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;

	forward.Normalize();
	right.Normalize();

	if (m_playerstate == enPlayerState_Walk||m_playerstate == enPlayerState_Idle)
	{
		//   X e B b N ̓  ͗ʂ 120.0f    Z B
		right *= stickL.x * 400.0f;
		forward *= stickL.y * 400.0f;
	}
	else if(m_playerstate == enPlayerState_PostureWalk || m_playerstate == enPlayerState_Attack)
	{
		right *= stickL.x * 140.0f;
		forward *= stickL.y * 140.0f;
	}
	else
	{
		right *= stickL.x * 0.0f;
		forward *= stickL.y * 0.0f;
	}

	//移動速度にスティックの入力量を加算する。
	m_moveSpeed += right + forward;

	//座標を移動
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//壁に突っ込んだ時に浮かび上がる現象が発生することがあるので、y座標を0にする
	m_position.y = 0.0f;

	//座標を設定。
	m_modelRender.SetPosition(m_position);
	m_charaCon.SetPosition(m_position);
}

void Player::Rolling()
{
	if (m_playerstate != enPlayerState_Rolling)
	{
		return;
	}

	m_rollingVec.Normalize();

	m_rollingVec *= 500.0f;
	
	m_rollingSpeed = m_rollingVec;

	m_rollingSpeed.y = 0.0f;

	m_position = m_charaCon.Execute(m_rollingSpeed, g_gameTime->GetFrameDeltaTime());

	//壁に突っ込んだ時に浮かび上がる現象が発生することがあるので、y座標を0にする
	m_position.y = 0.0f;

	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	m_modelRender.SetPosition(modelPosition);

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::Rotation()
{
	if (m_game->GetPlayerMove())
	{
		return;
	}

	//特定のステートの時は回転処理をしない
	if (m_playerstate == enPlayerState_Attack || m_playerstate == enPlayerState_PostureWalk
		||m_playerstate == enPlayerState_Rolling)
	{
		return;
	}

	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//絵描きさんに回転を教える。
		m_modelRender.SetRotation(m_rotation);
	}

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

}

void Player::Collision()
{
	//無敵時間が０秒の時
	if (m_muteki_timer <= 0.0f)
	{
		Vector3 tmp = m_position;
		tmp.y += 30.0f;
		m_collisionObject->SetPosition(tmp);


		//被ダメージステートまたはローリングステート時は当たり判定処理をしない
		if (m_playerstate == enPlayerState_ReceiveDamage || m_playerstate == enPlayerState_KnockDown)
		{
			return;
		}

		if (m_playerstate == enPlayerState_Rolling)
		{
			if (!m_justRollingCol)
			{
				return;
			}

			//プレイヤーの攻撃用のコリジョンを取得する。
			const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");

			//コリジョンの配列をfor文で回す。
			for (auto collision : collisions)
			{
				//コリジョンとキャラコンが衝突したら。
				if (collision->IsHit(m_justRollingCollisionObject))
				{
					m_remainingBulletsUi->IsReload(true);
					m_isReload = true;
					m_reloadTime = 0.3f;
					auto hgoe = NewGO<LanchSlowMotion>(0, "LanchSlowMotion");
					hgoe->Initialize(m_game);
					//m_game->SlowStart(0.1f);
				}
			}

			return;
		}
		//プレイヤーの攻撃用のコリジョンを取得する。
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
		//コリジョンの配列をfor文で回す。
		for (auto collision : collisions)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_collisionObject))
			{
				//HPを減らす
				m_game->m_hpui->DecreaseHP(10);
				//効果音を再生する
				SoundSource* m_hpEnemy = NewGO<SoundSource>(0);
				m_hpEnemy->Init(8);
				m_hpEnemy->Play(false);
				//ダメージ受けたとき、無敵状態のタイマー。
				m_muteki_timer = 3.0f;

				if (m_game->m_hpui->GetNowHP() <= 0)
				{
					DeleteGO(m_playerWalk);
					DeleteGO(m_playerStance);
					m_playerstate = enPlayerState_KnockDown;
				}
			}
		}
	}
	else
	{
		//無敵時間を進める
		m_muteki_timer -= g_gameTime->GetFrameDeltaTime();
	}
}

void Player::AttackRotation()
{
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	Vector3 MinVec = m_forward * 500.0f;

	bool EnemySearch = false;

	for (int ListnumA = 0; ListnumA < m_game->m_EnemyList.size(); ListnumA++) {
		Vector3 pos = m_game->m_EnemyList[ListnumA]->m_position;
		if (AngleCheck(m_forward,pos)) {
			Vector3 diffA = pos - m_position;
			if (diffA.Length() <= MinVec.Length()){
				MinVec = diffA;
				EnemySearch = true;
			}
		}
	}

	if (shot == false)
	{
		if (m_isReload)
		{
			return;
		}

		if (m_remainingBullets <= 0)
		{
			m_remainingBulletsUi->IsReload(true);
			m_isReload = true;
			m_reloadTime = 3.5f;
			return;
		}

		m_bullet = NewGO<Bullet>(0, "bullet");
		Quaternion rot;
		if (EnemySearch)
		{
			rot.SetRotation(Vector3::AxisZ, MinVec);
		}
		else
		{
			rot.SetRotation(Vector3::AxisZ,m_forward);
		}
		m_bullet->Setrotation(rot);
		m_bullet->SetMoveDirection(m_forward);
		m_bullet->SetPosition(m_position);
		m_bullet->SetShotType(Bullet::en_Player);
		m_remainingBullets--;
		shot = true;
	}	
}

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

bool Player::AngleCheck(const Vector3 froward ,const Vector3& position)
{
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	Vector3 diff = position - m_position;

	diff.Normalize();
	float angle = acosf(diff.Dot(froward));
	if (Math::PI * 0.07f <= fabsf(angle))
	{
		return false;
	}

	Vector3 EnemyPosition = position;
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はプレイヤーの座標
	start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
	//終点はエネミーの座標
	end.setOrigin(btVector3(EnemyPosition.x, EnemyPosition.y + 70.0f, EnemyPosition.z));

	SweepResultWall callback;
	//制作したコライダーを始点から終点まで動かして壁に接触したか判定
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//壁と衝突した時
	if (callback.isHit == true)
	{
		return false;
	}
	

	return true;
}

void Player::SetEvent(EnEvent eventnum)
{
	m_playerstate = enPlayerState_Event;

	for (int i = 0; i < sizeof(m_eventInfos) / sizeof(EventInfo); i++)
	{
		if (m_eventInfos[i].m_eventState == eventnum)
		{
			m_eventInfoNum = i;

			m_position = m_eventInfos[i].m_targetPos;

			m_charaCon.SetPosition(m_position);
			m_modelRender.SetPosition(m_position);

			break;
		}
	}
}

void Player::ManageState()
{
	switch (m_playerstate)
	{
	case Player::enPlayerState_Idle:
		ProcessIdleStateTransition();
		break;
	case Player::enPlayerState_Walk:
		ProcessWalkStateTransition();
		break;
	case Player::enPlayerState_Attack:
		ProcessAttackStateTransition();
		break;
	case Player::enPlayerState_PostureWalk:
		ProcessCommonStateTransition();
		break;
	case Player::enPlayerState_Rolling:
		ProcessRollingStateTransition();
		break;
	case Player::enPlayerState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
	default:
		break;
	}
}

void Player::PlayAnimation()
{
	switch (m_playerstate)
	{
	case Player::enPlayerState_Idle:
		//待機アニメーション
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case Player::enPlayerState_Walk:
		//歩き
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
	case Player::enPlayerState_Attack:
		//攻撃
		m_modelRender.SetAnimationSpeed(5.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Gunshot, 0.1f);
		break;
	case Player::enPlayerState_PostureWalk:
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f) {
			m_modelRender.SetAnimationSpeed(1.0f);
			m_modelRender.PlayAnimation(enAnimationClip_PostureWalk, 0.1f);
		}
		else
		{
			m_modelRender.SetAnimationSpeed(1.0f);
			m_modelRender.PlayAnimation(enAnimationClip_PostureIdle, 0.1f);
		}
		break;
	case Player::enPlayerState_Rolling:
		m_modelRender.SetAnimationSpeed(2.0);
		m_modelRender.PlayAnimation(enAnimationClip_Rolling, 0.1f);
		break;
	case Player::enPlayerState_ReceiveDamage:
		//被ダメージ
		m_modelRender.SetAnimationSpeed(3.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
	case Player::enPlayerState_KnockDown:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_knockdown, 0.1f);
	}
}

void Player::ProcessCommonStateTransition()
{
	if (m_game->GetPlayerMove())
	{
		return;
	}

	//Aボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_rollingCorrectionTime = 0.0f;

		float lStick_x = g_pad[0]->GetLStickXF();
		float lStick_y = g_pad[0]->GetLStickYF();
		if ((lStick_x <= 0.0f && lStick_x >= 0.0f)
			&& (lStick_y <= 0.0f && lStick_y >= 0.0f))
		{
			m_rollingVec = m_forward;
		}
		else
		{
			//左スティックの入力量を受け取る。
			//カメラの前方方向と右方向を取得。
			Vector3 cameraForward = g_camera3D->GetForward();
			Vector3 cameraRight = g_camera3D->GetRight();
			//XZ平面での前方方向、右方向に変換する。
			cameraForward.y = 0.0f;
			cameraForward.Normalize();
			cameraRight.y = 0.0f;
			cameraRight.Normalize();
			m_rollingVec += cameraForward * lStick_y * 200.0f;	//奥方向への移動速度を加算。
			m_rollingVec += cameraRight * lStick_x * 200.0f;		//右方向への移動速度を加算。
			//キャラクターの方向を変える。
			m_rotation.SetRotationYFromDirectionXZ(m_rollingVec);
		}
		//絵描きさんに回転を教える。
		m_modelRender.SetRotation(m_rotation);
		/*m_rollingVec = m_forward;*/
		//プレイヤーステートを回避にする
		m_playerstate = enPlayerState_Rolling;
		m_justRollingTime = 0.3f;
		m_justRollingCol = true;
		SoundSource* m_roPlayer = NewGO<SoundSource>(0);
		m_roPlayer->Init(12);
		m_roPlayer->Play(false);
		

		return;
	}

	if (g_pad[0]->IsTrigger(enButtonRB1))
	{
		//カメラの前方向と右方向のベクトルを持ってくる。
		Vector3 forward = g_camera3D->GetForward();
		Vector3 right = g_camera3D->GetRight();
		//y方向には移動させない。
		forward.y = 0.0f;
		right.y = 0.0f;

		forward.Normalize();
		right.Normalize();

		Vector3 stickL;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.z = g_pad[0]->GetLStickYF();

		right *= stickL.x;;
		forward *= stickL.z;

		Vector3 stickXY;

		stickXY += right + forward;

		stickXY.Normalize();

		Vector3 EnemyVec = stickXY * 500.0f;

		bool existenceEnemy = false;

		for (int ListnumA = 0; ListnumA < m_game->m_EnemyList.size(); ListnumA++) {
			Vector3 pos = m_game->m_EnemyList[ListnumA]->m_position;
			if (AngleCheck(stickXY, pos)) {
				Vector3 diffA = pos - m_position;
				if (diffA.Length() <= EnemyVec.Length()) {
					EnemyVec = diffA;
					existenceEnemy = true;
				}
			}
		}

		if (existenceEnemy)
		{
			Quaternion rot;
			rot.SetRotation(Vector3::AxisZ, EnemyVec);
			m_modelRender.SetRotation(rot);
			m_rotation = rot;
			m_forward = Vector3::AxisZ;
			rot.Apply(m_forward);
		}
	}
	if (g_pad[0]->IsTrigger(enButtonLB1) && !m_isReload)
	{
		if (!m_isReload || m_remainingBullets == 17)
		{
			m_remainingBulletsUi->IsReload(true);
			m_isReload = true;
			m_reloadTime = 3.0f;
		}
	}
	if (g_pad[0]->IsPress(enButtonRB1))
	{
		//構えなしの足音が再生中なら
		if (m_playerWalk->IsPlaying())
		{
			//構えなしの足音を停止させる
			m_playerWalk->Stop();
		}

		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			//構えありの足音が再生してなかったら
			if (!m_playerStance->IsPlaying())
			{
				//構えありの足音を再生させる
				m_playerStance->Play(true);
			}
		}

		if (fabsf(m_moveSpeed.x) == 0.0f && fabsf(m_moveSpeed.z) == 0.0f)
		{
			//構えありの足音が再生中なら
			if (m_playerStance->IsPlaying())
			{
				//構えありの足音を停止させる
				m_playerStance->Stop();
			}
		}

		if (g_pad[0]->IsTrigger(enButtonB))
		{
			m_playerstate = enPlayerState_Attack;
			SoundSource* m_atPlayer = NewGO<SoundSource>(0);
			m_atPlayer->Init(7);
			m_atPlayer->Play(false);
			return;
		}
		m_playerstate = enPlayerState_PostureWalk;
		// 
		if (g_pad[0]->IsTrigger(enButtonRB1))
		{
			//銃を構えた時の効果音を流す
			SoundSource* m_gunStance = NewGO<SoundSource>(0);
			m_gunStance->Init(13);
			m_gunStance->Play(false);
		}

		return;
	}
	//構えありの足音が再生中なら
	if (m_playerStance->IsPlaying())
	{
		//構えありの足音を停止させる
		m_playerStance->Stop();
	}

	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f) {

		//歩きステートにする
		m_playerstate = enPlayerState_Walk;
		// 足音が再生中じゃないなら
		if (!m_playerWalk->IsPlaying())
		{
			//足音を再生する
			m_playerWalk->Play(true);
			//m_playerWalk->SetVolume(2.0f);
		}
		
		return;
	}
	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else {
		//待機ステートにする。
		m_playerstate = enPlayerState_Idle;
		// 足音が再生中なら
		if (m_playerWalk->IsPlaying())
		{
			//足音を停止する
			m_playerWalk->Stop();
		}

		return;
	}
}

void Player::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void Player::ProcessWalkStateTransition()
{
	ProcessCommonStateTransition();
}

void Player::ProcessAttackStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false )
	{
		ProcessCommonStateTransition();
		shot = false;
	}
}

void Player::ProcessRollingStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		RollingEndRot();
		m_rollingCorrectionTime = 0.3f;
		ProcessCommonStateTransition();
	}
}

void Player::ProcessReceiveDamageStateTransition()
{
	//被ダメージモーションが終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		ProcessCommonStateTransition();
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"shot_point") == 0)
	{
		AttackRotation();
	}
}

void Player::RollingEndRot()
{

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;

	forward.Normalize();
	right.Normalize();

	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.z = g_pad[0]->GetLStickYF();

	right *= stickL.x;;
	forward *= stickL.z;

	Vector3 stickXY;

	stickXY += right + forward;

	stickXY.Normalize();

	Vector3 EnemyVec = stickXY * 500.0f;

	bool existenceEnemy = false;

	for (int ListnumA = 0; ListnumA < m_game->m_EnemyList.size(); ListnumA++) {
		Vector3 pos = m_game->m_EnemyList[ListnumA]->m_position;
		if (AngleCheck(stickXY, pos)) {
			Vector3 diffA = pos - m_position;
			if (diffA.Length() <= EnemyVec.Length()) {
				EnemyVec = diffA;
				existenceEnemy = true;
			}
		}
	}

	if (existenceEnemy)
	{
		Quaternion rot;
		rot.SetRotation(Vector3::AxisZ, EnemyVec);
		m_modelRender.SetRotation(rot);
		m_rotation = rot;
		m_forward = Vector3::AxisZ;
		rot.Apply(m_forward);
	}

	m_rollingVec = Vector3::Zero;
}



void Player::Render(RenderContext& rc)
{
	if (m_game->m_TempDelPlayer == false)
	{
		//無敵時間中出なければ
		if (m_muteki_timer <= 0.0f)
		{
			//モデルの描画。
			m_modelRender.Draw(rc);
		}	
		else
		{
			//無敵時間中は
			//モデルの表示非表示を毎フレーム切り替えて点滅
			if (m_mutekiModelDraw)
			{
				m_modelRender.Draw(rc);
				m_mutekiModelDraw = false;
			}
			else
			{
				m_mutekiModelDraw = true;
			}
		}
	}
}
