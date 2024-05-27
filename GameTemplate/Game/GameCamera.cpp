#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h";
#include "EventCamera.h"

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(0.0f, 150.0f, -300.0f);
	m_toCameraPos *= 0.5;
	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("player");

	m_eventCam = FindGO<EventCamera>("camera");
	
	//カメラのニアクリップとファークリップを設定。
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(30000.0f);

	return true;
}

void GameCamera::Update()
{
	if (m_eventCam->IsEvent())
		return;

	////注視点を計算
	//Vector3 target = m_player->m_position;
	////注視点 プレイヤーの座標に設定
	//target.y += 100.0f;

	//Vector3 playerBack = m_player->m_forward * -1;

	//Quaternion toPlayerBackRot;
	//toPlayerBackRot.SetRotation(m_toCameraPos, playerBack);

	//Vector3 tmp = m_toCameraPos;

	//toPlayerBackRot.Apply(tmp);

	//Vector3 pos = target + tmp;

	////メインカメラに注視点と視点を設定
	//g_camera3D->SetTarget(target);
	//g_camera3D->SetPosition(pos);

	////カメラの更新
	//g_camera3D->Update();

	///////////////////////////////////////////////////////////
	//カメラを更新
	//注視点を計算
	Vector3 target = m_player->m_position;
	//注視点 プレイヤーの座標に設定
	target.y += 70.0f;
	//視点を計算する。
	Vector3 toCameraPosOld = m_toCameraPos;

	//パッドの入力を使ってカメラを回す
	float x = g_pad[0]->GetRStickXF();
	//float y = g_pad[0]->GetRStickYF();
	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.7f * x);
	qRot.Apply(m_toCameraPos);
	//X軸周りの回転
	/*Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * y);
	qRot.Apply(m_toCameraPos);*/
	//カメラの回転の上限をチェックする。
	//注視点から視点までのベクトルを正規化する。
	//正規化すると、ベクトルの大きさが１になる。
	//大きさが１になる→ベクトルから強さがなくなり、方向のみの情報となるということ
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.2f) {
		//カメラが上向きすぎ
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		//カメラが下向きすぎ
		m_toCameraPos = toCameraPosOld;
	}

	//視点を計算する。
	Vector3 pos = target + m_toCameraPos;

	//メインカメラに注視点と視点を設定
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	//カメラの更新
	g_camera3D->Update();
}