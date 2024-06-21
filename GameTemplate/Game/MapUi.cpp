#include "stdafx.h"
#include "MapUi.h"

MapUi::MapUi()
{

}

MapUi::~MapUi()
{

}

namespace {
	const Vector3 n_position = { 630.0f,-270.0f,0.0f };
}

bool MapUi::Start()
{
	//マップフレーム画像を読み込む
	m_mapUiFrame.Init("Assets/modelData/ui_map/MapFrame.DDS", 300.0f, 300.0f);
	//位置を設定
	m_mapUiFrame.SetPosition(n_position);

	//マップベース画像を読み込む
	m_mapUiBase.Init("Assets/modelData/ui_map/MapBase.DDS", 300.0f, 300.0f);
	//位置を設定
	m_mapUiBase.SetPosition(n_position);

	//マッププレイヤーポインタ画像を読み込む
	m_mapUiPlayerPoint.Init("Assets/modelData/ui_map/MapPlayerPoint.DDS", 30.0f, 30.0f);
	//位置を設定
	m_mapUiPlayerPoint.SetPosition(n_position);

	m_game = FindGO<Game>("game");

	m_player = FindGO<Player>("player");

	CreateMapEnemyPoint();

	return true;
}

void MapUi::Update()
{
	//プレイヤーポインタの更新(主に回転)
	MapPlayerPointUpdate();

	m_mapUiBase.Update();
	m_mapUiFrame.Update();
	m_mapUiPlayerPoint.Update();
}

void MapUi::MapPlayerPointUpdate()
{
	Vector3 CamForward = g_camera3D->GetForward();
	CamForward.Normalize();
	CamForward.y = 0.0f;

	Vector3 camup2d;

	camup2d.x = CamForward.x;
	camup2d.y = CamForward.z;

	Vector3 playerforward = m_player->m_forward;

	playerforward.y = 0.0f;

	Vector3 uiplayerforward;
	uiplayerforward.x = playerforward.x;
	uiplayerforward.y = playerforward.z;

	Quaternion rot;
	rot.SetRotation(camup2d,uiplayerforward);

	m_mapUiPlayerPoint.SetRotation(rot);
}

void MapUi::CreateMapEnemyPoint()
{
	int nowenemynumber = m_game->GetEnemyListSize();

	for (int i = 0; i < nowenemynumber; i++)
	{
		std::unique_ptr<MapUi_EnemyPoint> newEnemyPoint(new MapUi_EnemyPoint());
		newEnemyPoint->SetEnemyInstance(m_game->GetEnemyListInstance(i));
	}
}

void MapUi::Render(RenderContext& rc)
{
	m_mapUiBase.Draw(rc);
	m_mapUiFrame.Draw(rc);
	m_mapUiPlayerPoint.Draw(rc);
}