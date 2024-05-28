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
	//0B��Bool�A000C���C�[�W���O�����A
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

	setVector.m_easingRatio = EasingTime;

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
	//0B��Bool�A000C���C�[�W���O�����A
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

	setVector.m_easingRatio = EasingTime;

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
	if (!m_cameraPosListEnd)
	{
		CamPositionUpdate();
	}

	//�^�[�Q�b�g�ʒu�X�V
	if (!m_cameraTarListEnd)
	{
		CamTargetUpdate();
	}

	Time();

	//�J�����ɍ��W�𑗂�
	g_camera3D->SetPosition(m_sendCameraPosition);
	g_camera3D->SetTarget(m_sendTargetPosition);

	if (m_cameraTarListEnd && m_cameraPosListEnd)
		m_eventFlag = false;
}

void EventCamera::CamPositionUpdate()
{
	if (m_posChangeTime <= 0.0f)
	{
		CamPositionListChange();
		if (m_cameraPosListEnd)
		return;
	}

	if (IsCamPosIteratorEasing(m_camPosListIterator))
	{
		m_sendCameraPosition = Easing(m_camPosListIterator,m_easingTimeCamPos);
	}
	else
	{
		m_sendCameraPosition = GetListPos(m_camPosListIterator);
	}


}

void EventCamera::CamTargetUpdate()
{
	if (m_tarChangeTime <= 0.0f)
	{
		CamTargetListChange();
		if (m_cameraTarListEnd)
			return;
	}

	if (IsCamTarIteratorEasing(m_camTarListIterator))
	{
		m_sendTargetPosition = Easing(m_camTarListIterator,m_easingTimeTarPos);
	}
	else
	{
		m_sendTargetPosition = GetListPos(m_camTarListIterator);
	}
}

void EventCamera::CamPositionListChange()
{
	m_camPosListIterator++;

	if (m_camPosListIterator == m_scene[m_sceneNow].m_cameraWayPoint.end())
	{
		m_cameraPosListEnd = true;

		return;
	}

	m_posChangeTime = m_camPosListIterator->second.m_changeTime;

	if (IsCamPosIteratorEasing(m_camPosListIterator))
	{
		m_easingPosRatio = m_camPosListIterator->second.m_easingRatio;
	}

	return;
}

void EventCamera::CamTargetListChange()
{
	m_camTarListIterator++;

	if (m_camTarListIterator == m_scene[m_sceneNow].m_cameraChangeTarget.end())
	{
		m_cameraTarListEnd = true;

		return;
	}

	m_tarChangeTime = m_camTarListIterator->second.m_changeTime;

	if (IsCamTarIteratorEasing(m_camPosListIterator))
	{
		m_easingTarRatio = m_camTarListIterator->second.m_easingRatio;
	}

	return;
}

Vector3 EventCamera::Easing(std::map<int, SceneVector>::iterator setIterator
,const float easingRatio)
{

	if (easingRatio >= 1.0f)
	{
		CamPositionListChange();
		return GetListPos(setIterator);;
	}

	Vector3 Last;

	Vector3 a = GetListPos(setIterator);

	Vector3 b = GetListPos(setIterator,1);

	Last.Lerp(easingRatio,a, b);

	return Last;
}

void EventCamera::Time()
{
	if (IsCamPosIteratorEasing(m_camPosListIterator))
	{
		m_easingTimeCamPos +=  g_gameTime->GetFrameDeltaTime() / m_easingPosRatio;
	}
	else
	{
		m_posChangeTime -= g_gameTime->GetFrameDeltaTime();
	}

	if (IsCamTarIteratorEasing(m_camTarListIterator))
	{
		m_easingTimeTarPos += g_gameTime->GetFrameDeltaTime() / m_easingTarRatio;
	}
	else
	{
		m_tarChangeTime -= g_gameTime->GetFrameDeltaTime();
	}
}