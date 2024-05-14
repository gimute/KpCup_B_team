#include "stdafx.h"
#include "SignalUi.h"
#include "GameCamera.h"
#include "SignalRailUi.h"
#include "Game.h"
#include "Enemy.h"
#include "Player.h"

namespace {
	const float DELETE_TIME = 1.0f;
}

SignalUi::SignalUi()
{

}

SignalUi::~SignalUi()
{

}

bool SignalUi::Start()
{
	m_SignalUi.Init("Assets/modelData/ui_signal/signal_UI_B.DDS",400.0f,400.0f);

	Vector3 pos = { 0.0f,-130.0f,0.0f };

	m_SignalUi.SetPosition(pos);

	m_gameCamera = FindGO<GameCamera>("gamecamera");

	m_signalRailUi = FindGO<SignalRailUi>("signalUi");

	m_game = FindGO<Game>("game");

	m_player = FindGO<Player>("player");

	m_VecNum = m_signalRailUi->m_signalQua;
	m_signalRailUi->m_signalQua++;
	m_signalRailUi->m_signalUiList.push_back(this);

	m_deleteTimer = DELETE_TIME;

	return true;
}

void SignalUi::Update()
{
	//‰ñ“]ˆ—
	Rotation();
	//íœˆ—
	DeleteSignal();
	//íœƒ^ƒCƒ}[ˆ—
	m_deleteTimer -= g_gameTime->GetFrameDeltaTime();
	//•`‰æˆ—
	m_SignalUi.Update();
}

void SignalUi::Rotation()
{
	Vector3 CamForward = g_camera3D->GetForward();
	CamForward.Normalize();
	CamForward.y = 0.0f;

	m_up2D.x = CamForward.x;
	m_up2D.y = CamForward.z;

	Vector3 CenterPos = m_player->GetPosition();

	Vector3 EnemyPos = m_game->m_EnemyList[m_enemyConnectNum]->m_position;

	Vector3 Diff = EnemyPos - CenterPos;

	Diff.Normalize();
	Diff.y = 0.0f;

	m_diff2D.x = Diff.x;
	m_diff2D.y = Diff.z;

	Quaternion rot;
	rot.SetRotation(m_up2D, m_diff2D);

	m_SignalUi.SetRotation(rot);
}

void SignalUi::DeleteSignal()
{
	if (m_game->m_EnemyList[m_enemyConnectNum]->m_signalBool == false
		|| m_game->m_EnemyList[m_enemyConnectNum]->m_enemystate != Enemy::enEnemyState_Attack 
		&& m_deleteTimer < 0.0f)
	{
		m_signalRailUi->DeleteSignalList(m_VecNum);
		DeleteGO(this);
	}
}

void SignalUi::Render(RenderContext& rc)
{
	m_SignalUi.Draw(rc);
}