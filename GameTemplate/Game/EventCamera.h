#pragma once

#define WAYPOINT_MAX 10
#define CHANGE_TARGET_MAX 10

class EventCamera :public IGameObject
{
public:
	/// <summary>
	/// �C�x���g�V�[��
	/// </summary>
	enum EnEventScene {
		en_Scene1_Door,
		en_Scene2_MapUp,
		en_SceneNum,
		en_Scene_None
	};
	/// <summary>
	/// ���X�g�X�V���[�h
	/// </summary>
	enum ListUpdateMode {
		en_ModePosition,
		en_ModeTarget,
	};
	/// <summary>
	/// �V�[�����Ŏg�����W�L�^�p�\����
	/// </summary>
	struct SceneVector
	{
		/// <summary>
		/// vector3�^
		/// </summary>
		Vector3 m_vector = Vector3::Zero;
		/// <summary>
		/// float�^1
		/// </summary>
		float m_changeTime = 0.0f;
		/// <summary>
		/// float�^2
		/// </summary>
		float m_easingRatio = 0.0f;
		/// <summary>
		/// ���̒n�_������̒n�_�ɃC�[�W���O�������̂ł����true
		/// </summary>
		bool isEasing = false;
		/// <summary>
		/// ���̒n�_�̃C�[�W���O���I�����Ă��邩�ǂ���
		/// </summary>
		bool isEasingEnd = false;
	};
	/// <summary>
	/// �P�V�[���Ŏg���ϐ��Ȃǂ��������\����
	/// </summary>
	struct OneScene{
		/// <summary>
		/// �J�����𓮂����ۂ̃E�F�C�|�C���g
		/// </summary>
		std::map<int, SceneVector> m_cameraWayPoint;
		/// <summary>
		/// �J�����̃^�[�Q�b�g�`�F���W
		/// </summary>
		std::map<int, SceneVector> m_cameraChangeTarget;
		/// <summary>
		/// �E�F�C�|�C���g���ǂꂾ���g����
		/// </summary>
		int m_useChangeTarget = 0;
		/// <summary>
		/// �^�[�Q�b�g�`�F���W���ǂꂾ���g����
		/// </summary>
		int m_useWayPoint = 0;
	};

public:
	/////////////////////////////////////////�֐�
	EventCamera();
	~EventCamera();
	/// <summary>
	/// �X�^�[�g�֐�
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// 00000000��0A 0B 000C 000D�Ƃ���0A���擪�ԍ�
	/// 0B��Bool�A000C���C�[�W���O�����A000D���؂�ւ��܂ł̕b���Ƃ���
	/// �w�肵���C�x���g�V�[���̗v�f���̃J�����ʒu�i�[�pmap�Ɋi�[���鏈���B
	/// </summary>
	/// <param name="setPos"></param>
	/// <param name="setNum"></param>
	/// <param name="setScene"></param>
	/// <param name="updateMode"></param>
	void SetSceneCamAndTarPos(const Vector3& setPos, const int setNum
		, const EnEventScene setScene, ListUpdateMode updateMode);
	/// <summary>
	/// �A�b�v�f�[�g�֐�
	/// </summary>
	void Update();
	/// <summary>
	/// �J�����ʒu�X�V
	/// </summary>
	Vector3 CamPositionUpdate(std::map<int, SceneVector>::iterator setIterator
	, const float easingRatio,ListUpdateMode updateMode);
	/// <summary>
	/// �J�����ʒu�z��X�V
	/// </summary>
	void CamPositionListChange(std::map<int, SceneVector>::iterator &setIterator
	,ListUpdateMode updateMode);
	/// <summary>
	/// �C�[�W���O����
	/// </summary>
	Vector3 Easing(std::map<int, SceneVector>::iterator setIterator
	,const float easingRatio);
	/// <summary>
	/// ���ԏ���
	/// </summary>
	void Time(std::map<int, SceneVector>::iterator setIterator
	, ListUpdateMode updateMode);
	/////////////////////////////////////////�����o�ϐ�
	/// <summary>
	/// ���x�������_�[
	/// </summary>
	LevelRender m_camLevelRender;
	/////////////////////////////////////////�ϐ�
	/// <summary>
	/// g_3Dcamera�ɃJ�����̍��W�𑗂邽�߂̕ϐ�
	/// </summary>
	Vector3 m_sendCameraPosition = Vector3::Zero;
	/// <summary>
	/// g_3Dcamera�ɃJ�����̃^�[�Q�b�g�̈ʒu�𑗂邽�߂̕ϐ�
	/// </summary>
	Vector3 m_sendTargetPosition = Vector3::Zero;
	/// <summary>
	/// �V�[��
	/// </summary>
	OneScene m_scene[EnEventScene::en_SceneNum];
	/// <summary>
	/// ���݃V�[��
	/// </summary>
	EnEventScene m_sceneNow = EnEventScene::en_Scene_None;
	/// <summary>
	/// �C�x���g���Đ����邩���Ȃ����̃t���O
	/// </summary>
	bool m_eventFlag = false;
	/// <summary>
	/// �J�������W�ύX�܂ł̎���
	/// </summary>
	float m_posChangeTime = 0.0f;
	/// <summary>
	/// �J�����^�[�Q�b�g���W�ύX�܂ł̎���
	/// </summary>
	float m_tarChangeTime = 0.0f;
	/// <summary>
	/// �J�����ʒu�C�[�W���O����
	/// </summary>
	float m_easingTimeCamPos = 0.0f;
	/// <summary>
	/// �J�����^�[�Q�b�g�ʒu�C�[�W���O����
	/// </summary>
	float m_easingTimeTarPos = 0.0f;
	/// <summary>
	/// �J�����ʒu�̃��X�g���I�����Ă��邩���Ă��Ȃ���
	/// </summary>
	bool m_cameraPosListEnd = false;
	/// <summary>
	/// �J�����^�[�Q�b�g�ʒu�̃��X�g���I�����Ă��邩���Ă��Ȃ���
    /// </summary>
	bool m_cameraTarListEnd = false;
	/// <summary>
	/// ���ݏ������̃J�����ʒu�̃C�e���[�^�[
	/// </summary>
	std::map<int, SceneVector>::iterator m_camPosListIterator;
	/// <summary>
	/// ���ݏ������̃J�����^�[�Q�b�g�ʒu�̃C�e���[�^�[
	/// </summary>
	std::map<int, SceneVector>::iterator m_camTarListIterator;
	/// <summary>
	/// �J�����ʒu�C�[�W���O����
	/// </summary>
	float m_easingPosRatio = 0.0f;
	/// <summary>
	/// �J�����^�[�Q�b�g�ʒu�C�[�W���O����
	/// </summary>
	float m_easingTarRatio = 0.0f;
	/// <summary>
	/// �J�����ʒu�̃C�[�W���O���I�����Ă��邩���Ȃ���
	/// </summary>
	bool m_camPoseasingEnd = false;
	/////////////////////////////////////////�����ݒ�n��
	/// <summary>
	/// �V�[���X�^�[�g
	/// </summary>
	/// <param name="setScene"></param>
	void StartScene(const EnEventScene setScene)
	{
		m_sceneNow = setScene;

		m_posChangeTime = m_scene[setScene].m_cameraWayPoint[0].m_changeTime;

		m_tarChangeTime = m_scene[setScene].m_cameraChangeTarget[0].m_changeTime;

		m_camPosListIterator = m_scene[setScene].m_cameraWayPoint.begin();

		m_camTarListIterator = m_scene[setScene].m_cameraChangeTarget.begin();

		m_easingPosRatio = m_scene[setScene].m_cameraWayPoint[0].m_easingRatio;

		m_easingTarRatio = m_scene[setScene].m_cameraChangeTarget[0].m_easingRatio;

		m_eventFlag = true;

		return;
	}
	/// <summary>
	/// ���݂̃C�e���[�^�[�ɓo�^����Ă���map�^�z��Ɋi�[����Ă�����W���擾����
	/// </summary>
	/// <param name="setIterator"></param>
	/// <returns></returns>
	Vector3 GetListPos(std::map<int, SceneVector>::iterator setIterator)
	{
		return setIterator->second.m_vector;
	}
	/// <summary>
	/// ���݂̃C�e���[�^�[�ɓo�^����Ă���map�^�z�񂩂�P�i�񂾗v�f�Ɋi�[����Ă�����W���擾����
	/// </summary>
	/// <param name="setIterator"></param>
	/// <param name="add"></param>
	/// <returns></returns>
	Vector3 GetListPos(std::map<int, SceneVector>::iterator setIterator, int add)
	{
		setIterator++;

		return setIterator->second.m_vector;
	}
	/// <summary>
	/// ���݂̃C�e���[�^�[�ɓo�^����Ă���map�^�z�񂩂�P�i�񂾗v�f�Ɋi�[����Ă�����W���擾����
	/// �P�i�񂾗v�f��end����Ό��̍��W�̎擾����
	/// </summary>
	/// <param name="setIterator"></param>
	/// <param name="add"></param>
	/// <param name="updateMode"></param>
	/// <returns></returns>
	Vector3 GetListPos(std::map<int, SceneVector>::iterator setIterator, int add
		, ListUpdateMode updateMode)
	{
		std::map<int, SceneVector>::iterator subIterator = setIterator;

		setIterator++;

		switch (updateMode)
		{
		case EventCamera::en_ModePosition:
			if (setIterator == m_scene[m_sceneNow].m_cameraWayPoint.end())
			{
				return subIterator->second.m_vector;
			}
			break;
		case EventCamera::en_ModeTarget:
			if (setIterator == m_scene[m_sceneNow].m_cameraChangeTarget.end())
			{
				return subIterator->second.m_vector;
			}
			break;
		}

		return setIterator->second.m_vector;
	}
	/// <summary>
	/// �C�x���g�Đ������ǂ���
	/// </summary>
	/// <returns></returns>
	bool IsEvent()
	{
		return m_eventFlag;
	}
	/// <summary>
	/// �J�����ʒu�̃C�e���[�^�[�̃C�[�W���O���I���ł��邩
	/// </summary>
	bool IsCamPosIteratorEasing(std::map<int, SceneVector>::iterator setIterator
		,ListUpdateMode updateMode)
	{
		switch (updateMode)
		{
		case EventCamera::en_ModePosition:
			if (m_camPosListIterator == m_scene[m_sceneNow].m_cameraWayPoint.end())
			{
				return false;
			}
			break;
		case EventCamera::en_ModeTarget:
			if (m_camTarListIterator == m_scene[m_sceneNow].m_cameraChangeTarget.end())
			{
				return false;
			}
			break;
		}

		return setIterator->second.isEasing;
	}

	bool IsIteratorEasingEnd(std::map<int, SceneVector>::iterator setIterator)
	{
		return setIterator->second.isEasingEnd;
	}
};

