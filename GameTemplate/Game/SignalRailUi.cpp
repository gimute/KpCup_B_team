#include "stdafx.h"
#include "SignalRailUi.h"
#include "SignalUi.h"
#include "Game.h"
#include "Enemy.h"

SignalRailUi::SignalRailUi()
{

}

SignalRailUi::~SignalRailUi()
{

}

bool SignalRailUi::Start()
{
	m_ReticleUi.Init("Assets/modelData/ui_signal/signal_UI_A.DDS", 400.0f, 400.0f);

	Vector3 pos = { 0.0f,-130.0f,0.0f };

	m_ReticleUi.SetPosition(pos);

	m_game = FindGO<Game>("game");

	return true;
}

void SignalRailUi::Update()
{
	//攻撃ステートEnemy探索処理
	AttackEnemySearch();
	//描画処理
	m_ReticleUi.Update();
}

void SignalRailUi::AttackEnemySearch()
{
	for (int Listnum = 0; Listnum < m_game->m_EnemyList.size(); Listnum++)
	{
		Vector3 enemypos = m_game->GetEnemyListPos(Listnum);

		if (EnemyStateIsAttack(Listnum)&& !ListCheck(Listnum)
			/*&& AngleCheck(enemypos)*/)
		{
			SignalUi* m_signalUi = NewGO<SignalUi>(3, "Ui");
			m_signalUi->SetEnemyVecNum(Listnum);
		}
	}
}

bool SignalRailUi::AngleCheck(const Vector3& position)
{
	Vector3 m_camForward = g_camera3D->GetForward();
	m_camForward.x *= -1;
	m_camForward.z *= -1;
	Vector3 m_camPosition = g_camera3D->GetPosition();

	Vector3 diff = position - m_camPosition;

	diff.Normalize();
	float angle = acosf(diff.Dot(m_camForward));
	if (Math::PI * 0.45f <= fabsf(angle))
	{
		return false;
	}

	return true;
}

bool SignalRailUi::ListCheck(int Listnum)
{
	//構造体配列を回す
	for (int i = 0; i < m_signalUiList.size(); i++)
	{
		//渡されたEnemyの配列番号と同じ番号が
		//m_camForwardListにあれば
		if (m_signalUiList[i]->m_enemyConnectNum == Listnum)
		{
			//trueを返す
			return true;
		}
	}

	//無ければfalseを返す
	return false;
}

bool SignalRailUi::EnemyStateIsAttack(int ListNum)
{
	if (m_game->m_EnemyList[ListNum]->m_enemyAttackStep == Enemy::en_stanceStep
		&& m_game->m_EnemyList[ListNum]->m_enemystate 
		== Enemy::enEnemyState_Attack)
	{
		return true;
	}
	return false;
}

void SignalRailUi::DeleteSignalList(const int num)
{
	m_signalUiList.erase(m_signalUiList.begin() + num);
	for (int VecNow = num; VecNow < m_signalUiList.size(); VecNow++) {
		m_signalUiList[VecNow]->m_VecNum -= 1;
	}
	m_signalQua--;
}

void SignalRailUi::Render(RenderContext& rc)
{
	if (m_game->m_TempDelSignalUi == true)
	{
		m_ReticleUi.Draw(rc);
	}
}