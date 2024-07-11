#include "stdafx.h"
#include "MapUi.h"

MapUi::MapUi()
{

}

MapUi::~MapUi()
{

}

namespace {
	const Vector3 MAP_POSITION = { 630.0f,-270.0f,0.0f };
	float MAP_RADIUS = 240.0f;
	float LIMITED_RANGE_IMAGE = 800.0f;
	float MAP_LIMITED_IMAGE = 300.0f;
}

bool MapUi::Start()
{
	//マップフレーム画像を読み込む
	m_mapUiFrame.Init("Assets/modelData/ui_map/MapFrame.DDS", 300.0f, 300.0f);
	//位置を設定
	m_mapUiFrame.SetPosition(MAP_POSITION);

	//マップベース画像を読み込む
	m_mapUiBase.Init("Assets/modelData/ui_map/MapBase.DDS", 300.0f, 300.0f);
	//位置を設定
	m_mapUiBase.SetPosition(MAP_POSITION);

	//マッププレイヤーポインタ画像を読み込む
	m_mapUiPlayerPoint.Init("Assets/modelData/ui_map/MapPlayerPoint.DDS", 30.0f, 30.0f);
	//位置を設定
	m_mapUiPlayerPoint.SetPosition(MAP_POSITION);

	//マップの目的地指すヤツポインタ(矢印)画像を読み込む
	m_mapNextDestinationPointArrow.Init("Assets/modelData/ui_map/MapNextDestinationPoint.DDS", 240.0f, 240.0f);
	//位置を設定
	m_mapNextDestinationPointArrow.SetPosition(MAP_POSITION);

	//マップの目的地指すヤツポインタ(〇)画像を読み込む
	m_mapNextDestInationPointCircle.Init("Assets/modelData/ui_map/MapNextDestinationPointPos.DDS", 30.0f, 30.0f);
	//位置を設定
	m_mapNextDestInationPointCircle.SetPosition(MAP_POSITION);

	m_game = FindGO<Game>("game");

	m_player = FindGO<Player>("player");

	CreateMapEnemyPoint(m_Initialization_Mode);

	return true;
}

void MapUi::Update()
{
	//プレイヤーポインタの更新(主に回転)
	MapPlayerPointUpdate();

	CreateMapEnemyPoint(m_Add_Mode);

	MapEnemyPointUpdate();

	EnemyPointDisplay();

	NextDestinationPointDisplay();

	m_mapUiBase.Update();
	m_mapUiFrame.Update();	
	for (const auto& ptr : m_mapUiEnemyPointList)
	{
		ptr.second->m_mapUiEnemyPoint.Update();
	}
	m_mapUiPlayerPoint.Update();
	m_mapNextDestinationPointArrow.Update();
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

void MapUi::MapEnemyPointUpdate()
{
	for (const auto& ptr : m_mapUiEnemyPointList)
	{
		Vector3 CamForward = g_camera3D->GetForward();
		CamForward.Normalize();
		CamForward.y = 0.0f;

		Vector3 camup2d;

		camup2d.x = CamForward.x;
		camup2d.y = CamForward.z;

		Vector3 playerforward = ptr.second->m_enemy->m_forward;

		playerforward.y = 0.0f;

		Vector3 uiplayerforward;
		uiplayerforward.x = playerforward.x;
		uiplayerforward.y = playerforward.z;

		Quaternion rot;
		rot.SetRotation(camup2d, uiplayerforward);

		ptr.second->m_mapUiEnemyPoint.SetRotation(rot);
	}
}

void MapUi::CreateMapEnemyPoint(const CreateMapEnemyPoint_Mode mode)
{
	if (mode == CreateMapEnemyPoint_Mode::m_Initialization_Mode)
	{
		//現在のエネミーの数を取得
		nowenemynumber = m_game->GetEnemyListSize();

		for (const auto& ptr : m_mapUiEnemyPointList)
		{
			delete ptr.second;
		}

		m_mapUiEnemyPointList.clear();

		//現在のエネミーの数分for文を回す
		for (int i = 0; i < nowenemynumber; i++)
		{
			//エネミーのポインタUiの構造体を宣言(コンストラクタで画像のファイルパスが登録されるため必要なし)
			MapUi_EnemyPoint* newEnemyPoint = new MapUi_EnemyPoint;
			//作成された構造体の変数にエネミーのインスタンスを登録
			newEnemyPoint->SetEnemyInstance(*m_game->GetEnemyListInstance(i));
			//登録が終わった変数を配列に記録
			m_mapUiEnemyPointList.insert({ i,newEnemyPoint });
			////ここで作った動的配列はスマートポインタに記録しておく
			//std::shared_ptr<MapUi_EnemyPoint> smart_ptr(newEnemyPoint);
		}

		return;
	}

	//前のエネミーの数を格納
	int oldEnemyNumber = nowenemynumber;

	//現在のエネミーの数を取得
	nowenemynumber = m_game->GetEnemyListSize();

	if (oldEnemyNumber > nowenemynumber)
	{

		std::vector<int> deleteNumber;

		for (const auto& ptr : m_mapUiEnemyPointList)
		{
			bool deadFlag = ptr.second->m_enemy->IsDead();

			if (deadFlag)
			{
				delete ptr.second;

				deleteNumber.push_back(ptr.first);
			}
		}

		if (deleteNumber.size() > 0)
		{
			for(const auto& ptr : deleteNumber)
			{
				m_mapUiEnemyPointList.erase(ptr);
			}
		}
	}

}

void MapUi::EnemyPointDisplay()
{
	Vector3 playerPos = m_player->GetPosition();
	Vector3 mapPos;

	//現在のエネミーの数分for文を回す
	for (const auto& ptr : m_mapUiEnemyPointList)
	{
		//配列に登録された構造体からエネミーの位置を取得
		Vector3 enemyPos = ptr.second->m_enemy->m_position;

		WorldPositionConvertToMapPosition(playerPos, enemyPos, mapPos);
		//スプライトに位置をセット
		ptr.second->m_mapUiEnemyPoint.SetPosition(mapPos);
		//境界線判定を行う
		ptr.second->m_isDraw = IsMapLimited(ptr.second
			->m_mapUiEnemyPoint.GetPosition());
	}
}

void MapUi::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition
	, Vector3& mapPosition)
{
	//Y座標はマップの座標とは関係ないので、0にする。
	worldCenterPosition.y = 0.0f;
	worldPosition.y = 0.0f;
	Vector3 diff = worldPosition - worldCenterPosition;

	//ベクトルの長さを取得。
	float length = diff.Length();

	//カメラの前方向ベクトルから。
	//クォータニオンを生成。
	Vector3 forward = g_camera3D->GetForward();
	Quaternion rot;
	rot.SetRotationY(atan2(-forward.x, forward.z));

	//ベクトルにカメラの回転を適用。
	rot.Apply(diff);

	//ベクトルを正規化。
	diff.Normalize();

	//マップの大きさ/距離制限で。
	//ベクトルをマップ座標系に変換する。
	diff *= length * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	//マップの中央座標と上記ベクトルを加算する。
	mapPosition = Vector3(MAP_POSITION.x + diff.x, MAP_POSITION.y + diff.z, 0.0f);
}

bool MapUi::IsMapLimited(const Vector3 determineTargetPos)
{
	//全体の比率から２わる
	float divisionLim = MAP_LIMITED_IMAGE / 2;
	//マップの中心点を取得
	Vector3 mapUiPos = MAP_POSITION;
	//マップの中心点に全体の比率から２を割ったものを足す
	//これが境界線となる
	Vector3 borderLimPlus;
	borderLimPlus.x = mapUiPos.x + divisionLim;
	borderLimPlus.y = mapUiPos.y + divisionLim;
	//さっき作ったのはプラスの分なのでマイナスの分も作る
	Vector3 bordarLimMinus;
	bordarLimMinus.x = mapUiPos.x - divisionLim;
	bordarLimMinus.y = mapUiPos.y - divisionLim;
	//境界線から出たらfalse
	if (determineTargetPos.x >= borderLimPlus.x || determineTargetPos.x <= bordarLimMinus.x
		|| determineTargetPos.y >= borderLimPlus.y || determineTargetPos.y <= bordarLimMinus.y)
	{
		return false;
	}
	//境界線内ならtrue
	return true;
}

void MapUi::NextDestinationPointDisplay()
{
	if (!m_isMapNextDestinationPointDraw)
	{
		return;
	}

	Vector3 playerPos = m_player->GetPosition();
	Vector3 mapPos;

	WorldPositionConvertToMapPosition(playerPos, m_mapNextDestinationPointPos, mapPos);

	m_mapNextDestInationPointCircle.SetPosition(mapPos);

	m_isMapNextDestinationPointDrawArrowOrCircle = IsMapLimited(m_mapNextDestInationPointCircle
		.GetPosition());

	m_mapNextDestInationPointCircle.Update();

	if (m_isMapNextDestinationPointDrawArrowOrCircle)
	{
		return;
	}

	Vector3 CamForward = g_camera3D->GetForward();
	CamForward.Normalize();
	CamForward.y = 0.0f;

	Vector3 camup2d;

	camup2d.x = CamForward.x;
	camup2d.y = CamForward.z;

	Vector3 PlayerPos = m_player->GetPosition();

	Vector3 NextDestinationForward = m_mapNextDestinationPointPos - PlayerPos;

	NextDestinationForward.y = 0.0f;

	NextDestinationForward.Normalize();

	Vector3 uiplayerforward;
	uiplayerforward.x = NextDestinationForward.x;
	uiplayerforward.y = NextDestinationForward.z;

	Quaternion rot;
	rot.SetRotation(camup2d,uiplayerforward);

	m_isMapNextDestinationPointDrawArrowOrCircle = false;

	m_mapNextDestinationPointArrow.SetRotation(rot);
}

void MapUi::Render(RenderContext& rc)
{
	if (m_game->m_TempDelHpUi == true) {
		m_mapUiBase.Draw(rc);
		m_mapUiFrame.Draw(rc);
		for (const auto& ptr : m_mapUiEnemyPointList)
		{
			if (ptr.second->m_isDraw)
			{
				ptr.second->m_mapUiEnemyPoint.Draw(rc);
			}
		}
		m_mapUiPlayerPoint.Draw(rc);
		if (m_isMapNextDestinationPointDraw)
		{
			if (m_isMapNextDestinationPointDrawArrowOrCircle)
			{
				m_mapNextDestInationPointCircle.Draw(rc);
			}
			else
			{
				m_mapNextDestinationPointArrow.Draw(rc);
			}
		}
	}
}