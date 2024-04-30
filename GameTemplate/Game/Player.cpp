#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "Game.h"
#include "Enemy.h"

Player::Player()
{

}

Player::~Player()
{

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
	m_animationclips[enAnimationClip_Gunshot].Load("Assets/modelData/player/proto_player/gunshot.tka");
	m_animationclips[enAnimationClip_Gunshot].SetLoopFlag(false);

	m_modelRender.Init("Assets/modelData/player/proto_player/proto_player2.tkm", m_animationclips, enAnimationClip_Num);

	m_charaCon.Init(25.0f, 40.0f, m_position);
	
	m_game = FindGO<Game>("game");

	return true;
}

void Player::Update()
{
	//移動処理。
	Move();
	//回転処理。
	Rotation();
	//アニメーション処理
	PlayAnimation();
	//ステートの遷移処理
	ManageState();
	//m_modelRender.SetPosition(30.0f, 0.0f, 0.0f);
	//モデルの更新。
	m_modelRender.Update();

}

void Player::Move()
{
	if (m_playerstate == enPlayerState_Attack)
		return;

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
	//左スティックの入力量と120.0fを乗算。
	right *= stickL.x * 200.0f;
	forward *= stickL.y * 200.0f;

	//移動速度にスティックの入力量を加算する。
	m_moveSpeed += right + forward;

	////地面に付いていたら。
	//if (m_charaCon.IsOnGround())
	//{
	//	//重力を無くす。
	//	m_moveSpeed.y = 0.0f;
	//	//LBボタンが押されたら。
	//	if (g_pad[0]->IsTrigger(enButtonLB1))
	//	{
	//		//ジャンプさせる。
	//		m_moveSpeed.y = 300.0f;
	//	}
	//}
	////地面に付いていなかったら。
	//else
	//{
	//	//重力を発生させる。
	//	m_moveSpeed.y -= 15.5f;
	//}
	//
	////Aボタン押されたら。
	//if (g_pad[0]->IsPress(enButtonA)) {
	//	//スピード上がっていく。
	//	if (m_speed < 600.0f) {
	//		m_speed += 3;
	//	}
	//	m_moveforward.Normalize();
	//	m_moveforward *= m_speed;
	//	//アクセル中にブレーキをかける。
	//	if (g_pad[0]->IsPress(enButtonB)) {	
	//		m_speed *= 0.95;
	//		m_moveforward.Normalize();
	//		m_moveforward *= m_speed;
	//	}
	//}
	////アクセル押さないでブレーキをかける。
	//else if (g_pad[0]->IsPress(enButtonB)) {
	//	m_speed *= 0.95;
	//	m_moveforward.Normalize();
	//	m_moveforward *= m_speed;
	//}
	////Aボタン押されなかったら。
	//else {
	//	//スピード落ちていく。
	//	if (m_speed >= 1) {
	//		m_speed -= 1;
	//		m_moveforward.Normalize();
	//		m_moveforward *= m_speed;
	//	}
	//}
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	m_modelRender.SetPosition(modelPosition);
	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	//座標を設定。
	m_modelRender.SetPosition(m_position);

}

void Player::Rotation()
{
	if (m_playerstate == enPlayerState_Attack)
		return;

	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//絵描きさんに回転を教える。
		m_modelRender.SetRotation(m_rotation);
	}

	//昔の処理
	//float Stick_y = g_pad[0]->GetLStickXF();
	//m_rotation.AddRotationY(Stick_y /= 100);
	//m_modelRender.SetRotation(m_rotation);
	//m_moveforward = Vector3::AxisZ;
	//m_rotation.Apply(m_moveforward);
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

}

void Player::AttackRotation()
{
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	Vector3 MinVec = m_forward * 500.0f;

	for (ListnumA = 0; ListnumA < m_game->m_EnemyList.size(); ListnumA++) {
		Vector3 pos = m_game->m_EnemyList[ListnumA]->m_position;
		if (AngleCheck(pos)) {
			Vector3 diffA = pos - m_position;
			if (diffA.Length() <= MinVec.Length()){
				MinVec = diffA;
			}
		}
	}

	m_bullet = NewGO<Bullet>(0, "bullet");
	Quaternion rot;
	rot.SetRotation(Vector3::AxisZ, MinVec);
	m_bullet->SetMoveDirection(m_forward);
	m_bullet->Setrotation(rot);
	m_bullet->SetPosition(m_position);
}

bool Player::AngleCheck(const Vector3& position)
{
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	Vector3 diff = position - m_position;

	diff.Normalize();
	float angle = acosf(diff.Dot(m_forward));
	//プレイヤーが視界内に居なかったら。
	if (Math::PI * 0.35f <= fabsf(angle))
	{
		//プレイヤーは見つかっていない。
		return false;
	}
	return true;
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
	}
}

void Player::PlayAnimation()
{
	switch (m_playerstate)
	{
	case Player::enPlayerState_Idle:
		//待機アニメーション
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case Player::enPlayerState_Walk:
		//歩き
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
	case Player::enPlayerState_Attack:
		//攻撃
		m_modelRender.PlayAnimation(enAnimationClip_Gunshot, 0.1f);
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		m_playerstate = enPlayerState_Attack;
		return;
	}
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f) {
		//歩きステートにする
		m_playerstate = enPlayerState_Walk;
		return;
	}
	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else {
		//待機ステートにする。
		m_playerstate = enPlayerState_Idle;
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
	AttackRotation();

	ProcessCommonStateTransition();
}

void Player::Render(RenderContext& rc)
{
	//モデルの描画。
	m_modelRender.Draw(rc);
}