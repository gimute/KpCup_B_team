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
	//�J�����ʒu���L�^���ꂽ���x����ǂݍ���
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
	//�\���̐錾
	SceneVector setVector;

	//0000��0A 0B 00C�Ƃ���0A���擪�ԍ�
	//0B��Bool�A00C���b���Ƃ���B

	//�擪�ԍ�
	int Num = setNum / 1000;

	//Bool�A0��1���Ŕ���
	int BoolNum = setNum % 1000;
	BoolNum /= 100;

	//�b��
	float Time = setNum % 100;
	Time *= 0.1;

	//���̕ϐ����\���̂Ɋi�[
	if (BoolNum == 0)
		setVector.isEasing = false;
	else
		setVector.isEasing = true;

	setVector.m_changeTime = Time;

	setVector.m_vector = setPos;

	//map�^�z��Ɋi�[
	m_scene[setScene].m_cameraWayPoint.insert({ Num,setVector });

	m_scene[setScene].m_useWayPoint++;
}

void EventCamera::SetSceneTarget(const Vector3& setPos, const int setNum
	, const EnEventScene setScene)
{
	//�\���̐錾
	SceneVector setVector;

	//0000��0A 0B 00C�Ƃ���0A���擪�ԍ�
	//0B��Bool�A00C���b���Ƃ���B

	//�擪�ԍ�
	int Num = setNum / 1000;

	//Bool�A0��1���Ŕ���
	int BoolNum = setNum % 1000;
	BoolNum /= 100;

	//�b��
	float Time = setNum % 100;
	Time *= 0.1;

	//���̕ϐ����\���̂Ɋi�[
	if (BoolNum == 0)
		setVector.isEasing = false;
	else
		setVector.isEasing = true;

	setVector.m_changeTime = Time;

	setVector.m_vector = setPos;

	//map�^�z��Ɋi�[
	m_scene[setScene].m_cameraChangeTarget.insert({ Num,setVector });

	m_scene[setScene].m_useChangeTarget++;
}

void EventCamera::Update()
{
	if (!m_eventFlag)
		return;

	//�J�����ʒu�X�V
	CamPositionUpdate();

	//�^�[�Q�b�g�ʒu�X�V
	CamTargetUpdate();

	//�J�����ɍ��W�𑗂�
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