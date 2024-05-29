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
		else if(objData.ForwardMatchName(L"B_Scene2Pos") == true)
		{
			SetSceneCamPos(objData.position, objData.number
				, EnEventScene::en_Scene2_MapUp);
		}
		else if (objData.ForwardMatchName(L"B_Scene2Tar") == true)
		{
			SetSceneTarget(objData.position, objData.number
				, EnEventScene::en_Scene2_MapUp);
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
	m_scene[setScene].m_cameraChangeTarget.insert({ Num,setVector });

	m_scene[setScene].m_useChangeTarget++;
}

void EventCamera::Update()
{
	//�C�x���g�t���O�������Ă��Ȃ����
	if (!m_eventFlag)
	{
		//�����͂��Ȃ�
		return;
	}

	//�J�����ʒu�ύX���Ԃ�0�ȉ���������
	if (m_posChangeTime <= 0.0f)
	{
		//�J�����ʒu�̃C�e���[�^�[�̗v�f��i�߂�
		CamPositionListChange(m_camPosListIterator
			,ListUpdateMode::en_ModePosition);
	}

	//�J�����^�[�Q�b�g�ʒu�ύX���Ԃ�0�ȉ���������
	if (m_tarChangeTime <= 0.0f)
	{
		//�J�����^�[�Q�b�g�ʒu�̃C�e���[�^�[�̗v�f��i�߂�
		CamPositionListChange(m_camTarListIterator
			, ListUpdateMode::en_ModeTarget);
	}

	//�J�����ʒu�X�V
	if (!m_cameraPosListEnd)
	{
		m_sendCameraPosition = CamPositionUpdate(m_camPosListIterator
			,m_easingTimeCamPos,ListUpdateMode::en_ModePosition);
	}

	//�^�[�Q�b�g�ʒu�X�V
	if (!m_cameraTarListEnd)
	{
		m_sendTargetPosition = CamPositionUpdate(m_camTarListIterator
			, m_easingTimeTarPos,ListUpdateMode::en_ModeTarget);
	}


	//�J�����ɍ��W�𑗂�
	g_camera3D->SetPosition(m_sendCameraPosition);
	g_camera3D->SetTarget(m_sendTargetPosition);

	if (m_cameraTarListEnd && m_cameraPosListEnd)
		m_eventFlag = false;
}

Vector3 EventCamera::CamPositionUpdate(std::map<int, SceneVector>::iterator setIterator
, const float easingTime,ListUpdateMode updateMode)
{
	Vector3 LastVector;

	if (IsCamPosIteratorEasing(setIterator,updateMode))
	{
		if (!IsIteratorEasingEnd(setIterator))
		{
			LastVector = Easing(setIterator, easingTime);
		}
		else
		{
			LastVector = GetListPos(setIterator, 1,updateMode);
		}
	}
	else
	{
		LastVector = GetListPos(setIterator);
	}	
	
	Time(setIterator,updateMode);
	return LastVector;
}

void EventCamera::CamPositionListChange(std::map<int, SceneVector>::iterator &setIterator
, ListUpdateMode updateMode)
{
	switch (updateMode)
	{
	case EventCamera::en_ModePosition:
		if (setIterator == m_scene[m_sceneNow].m_cameraWayPoint.end())
		{
			return;
		}
		break;
	case EventCamera::en_ModeTarget:
		if (setIterator == m_scene[m_sceneNow].m_cameraChangeTarget.end())
		{
			return;
		}
		break;
	}

	setIterator++;

	switch (updateMode)
	{
	case EventCamera::en_ModePosition:

		if (setIterator == m_scene[m_sceneNow].m_cameraWayPoint.end())
		{
			m_cameraPosListEnd = true;

			return;
		}

		m_posChangeTime = setIterator->second.m_changeTime;

		if (IsCamPosIteratorEasing(setIterator,ListUpdateMode::en_ModePosition))
		{
			m_easingPosRatio = m_camPosListIterator->second.m_easingRatio;
		}

		break;
	case EventCamera::en_ModeTarget:
		if (setIterator == m_scene[m_sceneNow].m_cameraChangeTarget.end())
		{
			m_cameraTarListEnd = true;

			return;
		}

		m_tarChangeTime = setIterator->second.m_changeTime;


		if (IsCamPosIteratorEasing(setIterator,ListUpdateMode::en_ModeTarget))
		{
			m_easingTarRatio = m_camTarListIterator->second.m_easingRatio;
		}

		break;

	}

	return;
}

Vector3 EventCamera::Easing(std::map<int, SceneVector>::iterator setIterator
,const float easingRatio)
{

	if (easingRatio >= 1.0f)
	{
		m_easingTimeCamPos = 0.0f;
		setIterator->second.isEasingEnd = true;
		return GetListPos(setIterator);
	}

	Vector3 Last;

	Vector3 a = GetListPos(setIterator);

	Vector3 b = GetListPos(setIterator,1);

	Last.Lerp(easingRatio,a, b);

	return Last;
}

void EventCamera::Time(std::map<int, SceneVector>::iterator setIterator
	, ListUpdateMode updateMode)
{
	switch (updateMode)
	{
	case EventCamera::en_ModePosition:
		if (!IsIteratorEasingEnd(m_camPosListIterator)
			&& IsCamPosIteratorEasing(setIterator, ListUpdateMode::en_ModePosition))
		{
			m_easingTimeCamPos += g_gameTime->GetFrameDeltaTime() / m_easingPosRatio;
		}
		else
		{
			m_posChangeTime -= g_gameTime->GetFrameDeltaTime();
		}
		break;
	case EventCamera::en_ModeTarget:
		if (!IsIteratorEasingEnd(m_camTarListIterator)
			&& IsCamPosIteratorEasing(setIterator, ListUpdateMode::en_ModeTarget))
		{
			m_easingTimeTarPos += g_gameTime->GetFrameDeltaTime() / m_easingTarRatio;
		}
		else
		{
			m_tarChangeTime -= g_gameTime->GetFrameDeltaTime();
		}
		break;
	}
}