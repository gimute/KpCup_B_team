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
		en_SceneNum,
		en_Scene_None
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
		float m_easingTime = 0.0f;
		/// <summary>
		/// bool�^
		/// </summary>
		bool isEasing = false;
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
	/// �A�b�v�f�[�g�֐�
	/// </summary>
	void Update();
	/// <summary>
	/// �J�����ʒu�X�V
	/// </summary>
	void CamPositionUpdate();
	/// <summary>
	/// �J�����^�[�Q�b�g�X�V
	/// </summary>
	void CamTargetUpdate();
	/// <summary>
	/// �J�����ʒu�z��X�V
	/// </summary>
	void CamPositionListChange();
	/// <summary>
	/// �J�����^�[�Q�b�g�ʒu�z��X�V
	/// </summary>
	void CamTargetListChange();
	/// <summary>
	/// ���ԏ���
	/// </summary>
	void Time();
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
	/// ���ݏ������̃J�����ʒu�̃C�e���[�^�[
	/// </summary>
	std::map<int, SceneVector>::iterator m_camPosListIterator;
	/// <summary>
	/// ���ݏ������̃J�����^�[�Q�b�g�ʒu�̃C�e���[�^�[
	/// </summary>
	std::map<int, SceneVector>::iterator m_camTarListIterator;
	/////////////////////////////////////////�����ݒ�n��
	void StartScene(const EnEventScene setScene)
	{
		m_sceneNow = setScene;

		m_posChangeTime = m_scene[setScene].m_cameraWayPoint[0].m_changeTime;

		m_tarChangeTime = m_scene[setScene].m_cameraChangeTarget[0].m_changeTime;

		m_camPosListIterator = m_scene[setScene].m_cameraWayPoint.begin();

		m_camTarListIterator = m_scene[setScene].m_cameraChangeTarget.begin();

		m_eventFlag = true;

		return;
	}
	/// <summary>
	/// ���x������ʒu��4���̔ԍ����擾���Ĉʒu�����̂܂܁A
	/// 4���̔ԍ���0A�擪�ԍ��A0Bbool�A00C�b���Ƃ���
	/// �w�肵���C�x���g�V�[���̗v�f���̃J�����ʒu�i�[�pmap
	/// �Ɋi�[���鏈���B
	/// </summary>
	/// <param name="setPos"></param>
	/// <param name="setNum"></param>
	/// <param name="setScene"></param>
	void SetSceneCamPos(const Vector3& setPos, const int setNum
		, const EnEventScene setScene);
	/// <summary>
	/// ���x������ʒu��4���̔ԍ����擾���Ĉʒu�����̂܂܁A
	/// 4���̔ԍ���0A�擪�ԍ��A0Bbool�A00C�b���Ƃ���
	/// �w�肵���C�x���g�V�[���̗v�f���̃J�����^�[�Q�b�g�ʒu�i�[�pmap
	/// �Ɋi�[���鏈���B
	/// </summary>
	/// <param name="setPos"></param>
	/// <param name="setNum"></param>
	/// <param name="setScene"></param>
	void SetSceneTarget(const Vector3& setPos, const int setNum
		, const EnEventScene setScene);
	/// <summary>
	/// �C�x���g�Đ������ǂ���
	/// </summary>
	/// <returns></returns>
	bool IsEvent()
	{
		return m_eventFlag;
	}
	bool IsIteratorEasing(std::map<int, SceneVector>::iterator setIterator)
	{
		return setIterator->second.isEasing;
	}
};

