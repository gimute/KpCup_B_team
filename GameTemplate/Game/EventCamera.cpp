#include "stdafx.h"
#include "EventCamera.h"
#include "Game.h"

namespace {
	int extractionNumListA[] = { 0,1,2,5,6 };
	int extractionNumListB[] = { 1,1,3,1,3 };
}

EventCamera::EventCamera()
{
	
}

EventCamera::~EventCamera()
{

}

bool EventCamera::Start()
{
	//�J�����ʒu���L�^���ꂽ���x����ǂݍ���
	m_camLevelRender.Init("Assets/levelData/eventCamTest.tkl", [&](LevelObjectData_Render& objData)
	{
		if (objData.ForwardMatchName(L"A_Scene1Pos") == true)
		{
			SetSceneCamAndTarPos(objData.position, objData.number
				, EnEventScene::en_Scene1_Door,ListUpdateMode::en_ModePosition);
			return true;
		}
		else if(objData.ForwardMatchName(L"A_Scene1Tar") == true)
		{
			SetSceneCamAndTarPos(objData.position, objData.number
				, EnEventScene::en_Scene1_Door, ListUpdateMode::en_ModeTarget);
		}
		else if(objData.ForwardMatchName(L"B_Scene2Pos") == true)
		{
			SetSceneCamAndTarPos(objData.position, objData.number
				, EnEventScene::en_Scene2_MapUp1, ListUpdateMode::en_ModePosition);
		}
		else if (objData.ForwardMatchName(L"B_Scene2Tar") == true)
		{
			SetSceneCamAndTarPos(objData.position, objData.number
				, EnEventScene::en_Scene2_MapUp1, ListUpdateMode::en_ModeTarget);
		}
		else if (objData.ForwardMatchName(L"C_Scene3Pos") == true)
		{
			SetSceneCamAndTarPos(objData.position, objData.number
				, EnEventScene::en_Scene3_MapUp2, ListUpdateMode::en_ModePosition);
		}
		else if (objData.ForwardMatchName(L"C_Scene3Tar") == true)
		{
			SetSceneCamAndTarPos(objData.position, objData.number
				, EnEventScene::en_Scene3_MapUp2, ListUpdateMode::en_ModeTarget);
		}
		return true;
	});

	//�Q�[���N���X�̃C���X�^���X���擾
	m_game = FindGO<Game>("game");

	return true;
}

void EventCamera::SetSceneCamAndTarPos(const Vector3& setPos, const int setNum
	, const EnEventScene setScene,ListUpdateMode updateMode)
{
	//�\���̐錾
	SceneVector setVector;

	std::string charStrA = std::to_string(setNum);

	charStrA.erase(charStrA.begin());

	int Num, BoolNum,IfNum;
	float EasingTime,ChangeTime;

	for (int i = 0; i < 5; i++)
	{
		//00000000��0A 0B 000C 000D�Ƃ���0A���擪�ԍ�
		//0B��Bool�A000C���C�[�W���O�����A0D���؂�ւ������̕t�^�A
		//000E���؂�ւ��܂ł̕b���Ƃ���B

		//�����ӁI
		//�؂�ւ��������I��(�؂�ւ������̕t�^�ϐ����P)�ɂȂ��Ă���ꍇ��
		//�؂�ւ��܂ł̕b��(000E)���؂�ւ������Ώۃi���o�[�ƂȂ�܂��B

		std::string charStrB = charStrA.substr(extractionNumListA[i],
			extractionNumListB[i]);

		switch (i)
		{
		case 0:
			//�擪�ԍ�
			Num = std::stoi(charStrB);
			break;		
		case 1:
			//bool�P���O���Ŕ���
			BoolNum = std::stoi(charStrB);
			break;		
		case 2:
			//�J�����C�[�W���O����
			EasingTime = std::stoi(charStrB);
			EasingTime *= 0.1;
			break;		
		case 3:
			//�؂�ւ������̕t�^
			IfNum = std::stoi(charStrB);
			break;
		case 4:
			//�J�����؂�ւ��b��
			ChangeTime = std::stoi(charStrB);
			if (IfNum == 0)
			{
				ChangeTime *= 0.1;
			}
			break;
		}
	}

	//���̕ϐ����\���̂Ɋi�[
	if (BoolNum == 0)
	{
		setVector.isEasing = false;
	}
	else
	{
		setVector.isEasing = true;
	}

	if (IfNum == 0)
	{
		setVector.isSwitchingCon = false;
	}
	else
	{
		setVector.isSwitchingCon = true;
	}

	setVector.m_changeTime = ChangeTime;

	setVector.m_easingRatio = EasingTime;

	setVector.m_vector = setPos;


	//enum�^�Ŋi�[�Ώۂ�ύX
	switch (updateMode)
	{
	case EventCamera::en_ModePosition:
		//�J�����ʒumap�^�z��Ɋi�[
		m_scene[setScene].m_cameraWayPoint.insert({ Num,setVector });

		m_scene[setScene].m_useWayPoint++;
		break;
	case EventCamera::en_ModeTarget:
		//�J�����^�[�Q�b�g�ʒumap�^�z��Ɋi�[
		m_scene[setScene].m_cameraChangeTarget.insert({ Num,setVector });

		m_scene[setScene].m_useChangeTarget++;
		break;
	}
}

void EventCamera::Update()
{
	//�C�x���g�t���O�������Ă��Ȃ����
	if (!m_eventFlag)
	{
		//�����͂��Ȃ�
		return;
	}

	//�J�����ʒu�ύX���Ԃ�0�ȉ���
	//�C�e���[�^�[���܂�end����Ȃ�������
	if (m_posChangeTime <= 0.0f && 
		!m_cameraPosListEnd)
	{
		//�J�����ʒu�̃C�e���[�^�[�̗v�f��i�߂�
		CamPositionListChange(m_camPosListIterator
			,ListUpdateMode::en_ModePosition);
	}

	//�J�����^�[�Q�b�g�ʒu�ύX���Ԃ�0�ȉ���
	//�C�e���[�^�[���܂�end����Ȃ�������
	if (m_tarChangeTime <= 0.0f && 
		!m_cameraTarListEnd)
	{
		//�J�����^�[�Q�b�g�ʒu�̃C�e���[�^�[�̗v�f��i�߂�
		CamPositionListChange(m_camTarListIterator
			, ListUpdateMode::en_ModeTarget);
	}

	//�C�e���[�^�[���܂�end����Ȃ�������
	if (!m_cameraPosListEnd)
	{
		//�J�����ʒu�X�V
		m_sendCameraPosition = CamPositionUpdate(m_camPosListIterator
			,m_easingRatioCamPos,ListUpdateMode::en_ModePosition);
	}

	//�C�e���[�^�[���܂�end����Ȃ�������
	if (!m_cameraTarListEnd)
	{
		//�^�[�Q�b�g�ʒu�X�V
		m_sendTargetPosition = CamPositionUpdate(m_camTarListIterator
			, m_easingRatioTarPos,ListUpdateMode::en_ModeTarget);
	}

	//�J�����ɍ��W�𑗂�
	g_camera3D->SetPosition(m_sendCameraPosition);
	g_camera3D->SetTarget(m_sendTargetPosition);

	//�J�����ʒu�A�J�����^�[�Q�b�g�ʒu�A�ǂ�����C�e���[�^�[��end��������
	if (m_cameraTarListEnd && m_cameraPosListEnd)
	{
		//�C�x���g���I������
		m_game->EventUiDelete(false);
		m_eventFlag = false;
	}
}

Vector3 EventCamera::CamPositionUpdate(std::map<int, SceneVector>::iterator setIterator
, const float easingTime,ListUpdateMode updateMode)
{
	//�ŏI�I�ɖ߂�l�Ƃ��ĕԂ��x�N�g��
	Vector3 LastVector;

	//���ݏ������̃C�e���[�^�[�̃C�[�W���O���I����������
	if (IsCamPosIteratorEasing(setIterator,updateMode))
	{
		//�����ăC�[�W���O���I�����Ă��Ȃ�������
		if (!IsIteratorEasingEnd(setIterator))
		{
			//�C�[�W���O�������ʂ�������
			LastVector = Easing(setIterator, easingTime
			,updateMode);
		}
		//�C�[�W���O���I�����Ă�����
		else
		{
			//���݂̃C�e���[�^�[����v�f���P�i�߂��C�e���[�^�[��
			//���W��������
			LastVector = GetListPos(setIterator, 1,updateMode);
		}
	}
	//�I�t��������
	else
	{
		//�C�e���[�^�[�Ɋi�[����Ă�����W����
		LastVector = GetListPos(setIterator);
	}	
	
	//�C�[�W���O�̊����v�Z�A�J�����؂�ւ��܂ł̎��Ԃ���������
	Time(setIterator,updateMode);

	return LastVector;
}

void EventCamera::CamPositionListChange(std::map<int, SceneVector>::iterator &setIterator
, ListUpdateMode updateMode)
{
	//�C�e���[�^�[�̗v�f���P�i�߂�
	setIterator++;

	//enum�^�A�b�v�f�[�g���[�h�ŕ���
	switch (updateMode)
	{

	//�A�b�v�f�[�g���[�h��position��������
	case EventCamera::en_ModePosition:

		//���݂̃C�e���[�^�[��end��������
		if (setIterator == m_scene[m_sceneNow].m_cameraWayPoint.end())
		{
			//end�t���O��true�ɂ���
			m_cameraPosListEnd = true;

			return;
		}


		//���݂̃C�e���[�^�[�̃J�����؂�ւ��������I���ɂȂ��Ă���Ȃ�
		if (setIterator->second.isSwitchingCon == true)
		{
			//�J�����^�[�Q�b�g�؂�ւ����Ԃ�1.0�ŏ�����
			m_posChangeTime = 1.0;
			//�J�����^�[�Q�b�g�؂�ւ������i���o�[��������
			m_camSwitchingNum = setIterator->second.m_changeTime;
		}
		//�I�t��������
		else
		{
			//�J�����ʒu�؂�ւ����Ԃ����݂̃C�e���[�^�[�ŏ�����
			m_posChangeTime = setIterator->second.m_changeTime;
		}


		//���݂̃C�e���[�^�[�̃C�[�W���O���I����������
		if (IsCamPosIteratorEasing(setIterator,ListUpdateMode::en_ModePosition))
		{
			//�C�[�W���O�̊��������݂̃C�e���[�^�[�ŏ�����
			m_easingPosRatio = m_camPosListIterator->second.m_easingRatio;
		}

		break;

		//�A�b�v�f�[�g���[�h��target��������
	case EventCamera::en_ModeTarget:

		//���݂̃C�e���[�^�[��end��������
		if (setIterator == m_scene[m_sceneNow].m_cameraChangeTarget.end())
		{			
			//end�t���O��true�ɂ���
			m_cameraTarListEnd = true;

			return;
		}

		//���݂̃C�e���[�^�[�̃J�����؂�ւ��������I���ɂȂ��Ă���Ȃ�
		if (setIterator->second.isSwitchingCon == true)
		{
			//�J�����^�[�Q�b�g�؂�ւ����Ԃ�1.0�ŏ�����
			m_tarChangeTime = 1.0;
			//�J�����^�[�Q�b�g�؂�ւ������i���o�[��������
			m_tarSwitchingNum = setIterator->second.m_changeTime;
		}
		//�I�t��������
		else
		{
			//�J�����^�[�Q�b�g�؂�ւ����Ԃ����݂̃C�e���[�^�[�ŏ�����
			m_tarChangeTime = setIterator->second.m_changeTime;
		}

		//���݂̃C�e���[�^�[�̃C�[�W���O���I����������
		if (IsCamPosIteratorEasing(setIterator,ListUpdateMode::en_ModeTarget))
		{
			//�C�[�W���O�̊��������݂̃C�e���[�^�[�ŏ�����
			m_easingTarRatio = m_camTarListIterator->second.m_easingRatio;
		}

		break;

	}

	return;
}

Vector3 EventCamera::Easing(std::map<int, SceneVector>::iterator setIterator
,float easingRatio,ListUpdateMode updateMode)
{

	//�C�[�W���O�������ő�ɂȂ�����
	if (easingRatio >= 1.0f)
	{
		easingRatio = 1.0f;
		//�C�[�W���O������������
		EasingClear(updateMode);
		//�C�[�W���O�I���t���O��true��
		setIterator->second.isEasingEnd = true;
	}

	//�R�R���牺�̓C�[�W���O�̏���
	Vector3 Last;

	Vector3 a = GetListPos(setIterator);

	Vector3 b = GetListPos(setIterator,1);

	Last.Lerp(easingRatio,a, b);

	return Last;
}

void EventCamera::Time(std::map<int, SceneVector>::iterator setIterator
	, ListUpdateMode updateMode)
{
	//�؂�ւ��������i���o�[�̎��̏���
	if (setIterator->second.isSwitchingCon == true)
	{
		switch (updateMode)
		{
		case EventCamera::en_ModePosition:
			//���ݏ������̃C�e���[�^�[�̃C�[�W���O���I�����Ă��炸
			//���ݏ������̃C�e���[�^�[�̃C�[�W���O���I���ł����
			if (!IsIteratorEasingEnd(m_camPosListIterator)
				&& IsCamPosIteratorEasing(setIterator, ListUpdateMode::en_ModePosition))
			{
				//�C�[�W���O�̊����𑝂₷
				m_easingRatioCamPos += g_gameTime->GetFrameDeltaTime() * m_easingPosRatio;
			}
			//��L�̏����ƍ���Ȃ����
			else
			{
				//�����J�����^�[�Q�b�g�̃i���o�[��
				//�w�肵�������i���o�[��������
				if (m_camTarListIterator->first == m_camSwitchingNum)
				{
					//�J�����ʒu�؂�ւ��^�C�}�[��0.0f�ɂ���
					m_posChangeTime = 0.0f;
				}
			}
			break;
		case EventCamera::en_ModeTarget:
			//���ݏ������̃C�e���[�^�[�̃C�[�W���O���I�����Ă��炸
			//���ݏ������̃C�e���[�^�[�̃C�[�W���O���I���ł����
			if (!IsIteratorEasingEnd(m_camTarListIterator)
				&& IsCamPosIteratorEasing(setIterator, ListUpdateMode::en_ModeTarget))
			{
				//�C�[�W���O�̊����𑝂₷
				m_easingRatioTarPos += g_gameTime->GetFrameDeltaTime() * m_easingTarRatio;
			}
			//��L�̏����ƍ���Ȃ����
			else
			{
				//�����J�����^�[�Q�b�g�̃i���o�[��
				//�w�肵�������i���o�[��������
				if (m_camPosListIterator->first == m_tarSwitchingNum)
				{
					//�J�����ʒu�؂�ւ��^�C�}�[��0.0f�ɂ���
					m_tarChangeTime = 0.0f;
				}
			}
			break;
		}
		return;
	}

	//�؂�ւ����������Ԃ̎��̏���
	//	//enum�^�A�b�v�f�[�g���[�h�ŕ���
	switch (updateMode)
	{

	//�A�b�v�f�[�g���[�h��position��������
	case EventCamera::en_ModePosition:

		//���ݏ������̃C�e���[�^�[�̃C�[�W���O���I�����Ă��炸
		//���ݏ������̃C�e���[�^�[�̃C�[�W���O���I���ł����
		if (!IsIteratorEasingEnd(m_camPosListIterator)
			&& IsCamPosIteratorEasing(setIterator, ListUpdateMode::en_ModePosition))
		{
			//�C�[�W���O�̊����𑝂₷
			m_easingRatioCamPos += g_gameTime->GetFrameDeltaTime() * m_easingPosRatio;
		}
		//��L�̏����ƍ���Ȃ����
		else
		{
			//�J�����ʒu�؂�ւ����Ԃ����炷
			m_posChangeTime -= g_gameTime->GetFrameDeltaTime();
		}
		break;

	//�A�b�v�f�[�g���[�h��position��������
	case EventCamera::en_ModeTarget:

		//���ݏ������̃C�e���[�^�[�̃C�[�W���O���I�����Ă��炸
		//���ݏ������̃C�e���[�^�[�̃C�[�W���O���I���ł����
		if (!IsIteratorEasingEnd(m_camTarListIterator)
			&& IsCamPosIteratorEasing(setIterator, ListUpdateMode::en_ModeTarget))
		{
			//�C�[�W���O�̊����𑝂₷
			m_easingRatioTarPos += g_gameTime->GetFrameDeltaTime() * m_easingTarRatio;
		}
		else
		{
			//�J�����^�[�Q�b�g�ʒu�؂�ւ����Ԃ����炷
			m_tarChangeTime -= g_gameTime->GetFrameDeltaTime();
		}
		break;
	}
}