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

	//00000000��0A 0B 000C 000D�Ƃ���0A���擪�ԍ�
	//0B��Bool�A000C���C�[�W���O�b���A
	//000D���؂�ւ��܂ł̕b���Ƃ���B

	//�擪�ԍ�
	int Num = setNum / 10000000;

	//Bool�A0��1���Ŕ���
	int BoolNum = setNum % 10000000;
	BoolNum /= 1000000;

	//�J�����C�[�W���O���x�b��
	float EasingTime = (setNum / 1000) % 1000;
	EasingTime *= 0.1;

	//�J�����؂�ւ��b��
	float ChangeTime = setNum % 100;
	ChangeTime *= 0.1;

	//���̕ϐ����\���̂Ɋi�[
	if (BoolNum == 0)
		setVector.isEasing = false;
	else
		setVector.isEasing = true;

	setVector.m_changeTime = ChangeTime;

	setVector.m_easingTime = EasingTime;

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

	//00000000��0A 0B 000C 000D�Ƃ���0A���擪�ԍ�
	//0B��Bool�A000C���C�[�W���O�b���A
	//000D���؂�ւ��܂ł̕b���Ƃ���B

	//�擪�ԍ�
	int Num = setNum / 1000;

	//Bool�A0��1���Ŕ���
	int BoolNum = setNum % 1000;
	BoolNum /= 100;

	//�J�����C�[�W���O���x�b��
	float EasingTime = (setNum / 1000) % 1000;
	EasingTime *= 0.1;

	//�J�����C�[�W���O���x�b��
	float ChangeTime = setNum % 100;
	ChangeTime *= 0.1;

	//���̕ϐ����\���̂Ɋi�[
	if (BoolNum == 0)
		setVector.isEasing = false;
	else
		setVector.isEasing = true;

	setVector.m_changeTime = ChangeTime;

	setVector.m_easingTime = EasingTime;

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