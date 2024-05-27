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

	//00000000を0A 0B 000C 000Dとして0Aが先頭番号
	//0BがBool、000Cがイージング秒数、
	//000Dが切り替えまでの秒数とする。

	//先頭番号
	int Num = setNum / 10000000;

	//Bool、0か1かで判定
	int BoolNum = setNum % 10000000;
	BoolNum /= 1000000;

	//カメライージング速度秒数
	float EasingTime = (setNum / 1000) % 1000;
	EasingTime *= 0.1;

	//カメラ切り替え秒数
	float ChangeTime = setNum % 100;
	ChangeTime *= 0.1;

	//↑の変数を構造体に格納
	if (BoolNum == 0)
		setVector.isEasing = false;
	else
		setVector.isEasing = true;

	setVector.m_changeTime = ChangeTime;

	setVector.m_easingTime = EasingTime;

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

	//00000000を0A 0B 000C 000Dとして0Aが先頭番号
	//0BがBool、000Cがイージング秒数、
	//000Dが切り替えまでの秒数とする。

	//先頭番号
	int Num = setNum / 1000;

	//Bool、0か1かで判定
	int BoolNum = setNum % 1000;
	BoolNum /= 100;

	//カメライージング速度秒数
	float EasingTime = (setNum / 1000) % 1000;
	EasingTime *= 0.1;

	//カメライージング速度秒数
	float ChangeTime = setNum % 100;
	ChangeTime *= 0.1;

	//↑の変数を構造体に格納
	if (BoolNum == 0)
		setVector.isEasing = false;
	else
		setVector.isEasing = true;

	setVector.m_changeTime = ChangeTime;

	setVector.m_easingTime = EasingTime;

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
	if (m_posChangeTime <= 0.0f)
	{
		CamPositionListChange();
		return;
	}

	if ()
	{

	}
}

void EventCamera::CamTargetUpdate()
{

}

void EventCamera::CamPositionListChange()
{
	m_camPosListIterator++;

	m_posChangeTime = m_camPosListIterator->second.m_changeTime;

	return;
}

void EventCamera::CamTargetListChange()
{
	m_camTarListIterator++;

	m_tarChangeTime = m_camTarListIterator->second.m_changeTime;

	return;
}

void EventCamera::Time()
{

}