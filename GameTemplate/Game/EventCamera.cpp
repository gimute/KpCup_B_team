#include "stdafx.h"
#include "EventCamera.h"

EventCamera::EventCamera()
{
	
}

EventCamera::~EventCamera()
{

}

bool EventCamera::Start()
{
	//SetScenePosInstance(en_Scene1_Door, SCENE_1_VECTOR);
	//カメラ位置が記録されたレベルを読み込む
	m_camLevelRender.Init("Assets/levelData/eventCamPosTar.tkl", [&](LevelObjectData_Render& objData)
	{
		if (objData.ForwardMatchName(L"A_Scene1Pos") == true)
		{
			SetSceneCamPos(objData.position, objData.number
				, EnEventScene::en_Scene1_Door);
			return true;
		}
		else if(objData.ForwardMatchName(L"A_Scene1Tar") == true)
		{
			SetSceneTarget(objData.position, objData.number
				, EnEventScene::en_Scene1_Door);
		}
		return true;
	});

	return true;
}

void EventCamera::SetSceneCamPos(const Vector3& setPos, const int setNum
	, const EnEventScene setScene)
{
	//構造体宣言
	SceneVector setVector;

	//0000を0A 0B 00Cとして0Aが先頭番号
	//0BがBool、00Cが秒数とする。

	//先頭番号
	int Num = setNum / 1000;

	//Bool、0か1かで判定
	int BoolNum = setNum % 1000;
	BoolNum /= 100;

	//秒数
	float Time = setNum % 100;
	Time *= 0.1;

	//↑の変数を構造体に格納
	if (BoolNum == 0)
		setVector.isEasing = false;
	else
		setVector.isEasing = true;

	setVector.m_changeTime = Time;

	setVector.m_vector = setPos;

	//map型配列に格納
	m_scene[setScene].m_cameraWayPoint.insert({ Num,setVector });

	m_scene[setScene].m_useWayPoint++;
}

void EventCamera::SetSceneTarget(const Vector3& setPos, const int setNum
	, const EnEventScene setScene)
{
	//構造体宣言
	SceneVector setVector;

	//0000を0A 0B 00Cとして0Aが先頭番号
	//0BがBool、00Cが秒数とする。

	//先頭番号
	int Num = setNum / 1000;

	//Bool、0か1かで判定
	int BoolNum = setNum % 1000;
	BoolNum /= 100;

	//秒数
	float Time = setNum % 100;
	Time *= 0.1;

	//↑の変数を構造体に格納
	if (BoolNum == 0)
		setVector.isEasing = false;
	else
		setVector.isEasing = true;

	setVector.m_changeTime = Time;

	setVector.m_vector = setPos;

	//map型配列に格納
	m_scene[setScene].m_cameraChangeTarget.insert({ Num,setVector });

	m_scene[setScene].m_useChangeTarget++;
}

void EventCamera::Update()
{
	if (!m_eventFlag)
		return;

	//カメラ位置更新
	CamPositionUpdate();

	//ターゲット位置更新
	CamTargetUpdate();

	//カメラに座標を送る
	g_camera3D->SetPosition(m_sendCameraPosition);
	g_camera3D->SetTarget(m_sendTargetPosition);
}

void EventCamera::CamPositionUpdate()
{
	int i = 0;

	//m_sendCameraPosition = m_scene[m_sceneNow].m_cameraWayPoint[i];
}

void EventCamera::CamTargetUpdate()
{
	int i = 0;

	//m_sendTargetPosition = m_scene[m_sceneNow].m_cameraChangeTarget[i];
}