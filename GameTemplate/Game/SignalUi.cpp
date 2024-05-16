#include "stdafx.h"
#include "SignalUi.h"
#include "GameCamera.h"
#include "SignalRailUi.h"
#include "Game.h"
#include "Enemy.h"
#include "Player.h"

SignalUi::SignalUi()
{

}

SignalUi::~SignalUi()
{

}

bool SignalUi::Start()
{
	m_CautionSignalUi.Init("Assets/modelData/ui_signal/signal_Ui_C_1.DDS",400.0f,400.0f);

	//SpriteInitData initDataCautionSigUi;

	//initDataCautionSigUi.m_ddsFilePath[0] = "Assets/modelData/ui_signal/signal_Ui_C_1.DDS";

	//m_cautionSignalTexture.InitFromDDSFile(L"Assets/modelData/ui_signal/signal_Ui_B_2.DDS");
	//initDataCautionSigUi.m_expandShaderResoruceView[0] = &m_cautionSignalTexture;

	//initDataCautionSigUi.m_fxFilePath = "Assets/shader/spriteSignalUi.fx";

	//initDataCautionSigUi.m_expandConstantBuffer = &m_alpha;
	//initDataCautionSigUi.m_expandConstantBufferSize = sizeof(float);

	//initDataCautionSigUi.m_width = static_cast<UINT>(400.0f);
	//initDataCautionSigUi.m_height = static_cast<UINT>(400.0f);

	//initDataCautionSigUi.m_alphaBlendMode = AlphaBlendMode_Trans;

	//m_CautionSignalUi.Init(initDataCautionSigUi);

	Vector3 pos = { 0.0f,-130.0f,0.0f };

	m_CautionSignalUi.SetPosition(pos);

	SpriteInitData initDataDangerSigUi;

	initDataDangerSigUi.m_ddsFilePath[0] = "Assets/modelData/ui_signal/signal_Ui_B_1.DDS";

	m_dangerSignalTexture.InitFromDDSFile(L"Assets/modelData/ui_signal/signal_Ui_B_2.DDS");
	initDataDangerSigUi.m_expandShaderResoruceView[0] = &m_dangerSignalTexture;

	initDataDangerSigUi.m_fxFilePath = "Assets/shader/spriteSignalUi.fx";

	initDataDangerSigUi.m_expandConstantBuffer = &m_alpha;
	initDataDangerSigUi.m_expandConstantBufferSize = sizeof(float);

	initDataDangerSigUi.m_width = static_cast<UINT>(400.0f);
	initDataDangerSigUi.m_height = static_cast<UINT>(400.0f);

	initDataDangerSigUi.m_alphaBlendMode = AlphaBlendMode_Trans;
	
	m_DangerSignalUi.Init(initDataDangerSigUi);

	m_DangerSignalUi.SetPosition(pos);

	m_gameCamera = FindGO<GameCamera>("gamecamera");

	m_signalRailUi = FindGO<SignalRailUi>("signalUi");

	m_game = FindGO<Game>("game");

	m_player = FindGO<Player>("player");

	m_VecNum = m_signalRailUi->m_signalQua;
	m_signalRailUi->m_signalQua++;
	m_signalRailUi->m_signalUiList.push_back(this);

	return true;
}

void SignalUi::Update()
{
	//削除処理
	DeleteSignal();
	//回転処理
	Rotation();
	//アルファ値の計算処理
	AlphaCalc();
	//削除タイマー処理
	m_deleteTimer -= g_gameTime->GetFrameDeltaTime();
	//描画処理
	m_DangerSignalUi.Update();
	m_CautionSignalUi.Update();
}

void SignalUi::Rotation()
{

	Vector3 CamForward = g_camera3D->GetForward();
	CamForward.Normalize();
	CamForward.y = 0.0f;

	m_up2D.x = CamForward.x;
	m_up2D.y = CamForward.z;

	Vector3 CenterPos = m_player->GetPosition();

	Vector3 EnemyPos = m_game->GetEnemyListPos(m_enemyConnectNum);

	Vector3 Diff = EnemyPos - CenterPos;

	Diff.Normalize();
	Diff.y = 0.0f;

	m_diff2D.x = Diff.x;
	m_diff2D.y = Diff.z;

	Quaternion rot;
	rot.SetRotation(m_up2D, m_diff2D);

	m_DangerSignalUi.SetRotation(rot);
	m_CautionSignalUi.SetRotation(rot);
}

void SignalUi::DeleteSignal()
{
	if (m_game->EnemyListExistence(m_enemyConnectNum)
		|| m_game->m_EnemyList[m_enemyConnectNum]->m_enemyAttackStep == Enemy::en_noneStep
		|| m_game->m_EnemyList[m_enemyConnectNum]->m_enemystate != Enemy::enEnemyState_Attack)
	{
		m_signalRailUi->DeleteSignalList(m_VecNum);
		DeleteGO(this);
	}
}

void SignalUi::AlphaCalc()
{
	if (m_alpha >= 1.0f)
	{
		m_alphaCalcBool = false;
	}

	if (m_alphaCalcBool)
	{
		m_alpha += 0.1f;
		return;
	}

	m_alpha -= 0.1f;

	if (m_alpha <= 0.1f)
	{
		m_alphaCalcBool = true;
	}
}

bool SignalUi::EnemyAttackStep()
{
	if (m_game->m_EnemyList[m_enemyConnectNum]->m_enemyAttackStep
		== Enemy::en_stanceStep)
	{
		return true;
	}
	else if (m_game->m_EnemyList[m_enemyConnectNum]->m_enemyAttackStep
		== Enemy::en_shotStep)
	{
		return false;
	}
}

void SignalUi::Render(RenderContext& rc)
{
	if (EnemyAttackStep())
	{
		m_CautionSignalUi.Draw(rc);
	}
	else
	{
		m_DangerSignalUi.Draw(rc);
	}
}