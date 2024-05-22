#pragma once

#define WAYPOINT_MAX 10
#define CHANGE_TARGET_MAX 10

class EventCamera :public IGameObject
{
	enum EnEventScene {
		en_Scene1_Door,
		en_SceneNum,
		en_Scene_None
	};

	struct OneScene{
		/// <summary>
		/// �J�����𓮂����ۂ̃E�F�C�|�C���g
		/// </summary>
		Vector3 m_cameraWayPoint[WAYPOINT_MAX];
		/// <summary>
		/// �J�����̃^�[�Q�b�g�`�F���W
		/// </summary>
		Vector3 m_cameraChangeTarget[CHANGE_TARGET_MAX];
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
	/////////////////////////////////////////�����o�ϐ�
	/////////////////////////////////////////�ϐ�
	/// <summary>
	/// �V�[��
	/// </summary>
	OneScene m_scene[EnEventScene::en_SceneNum];
	/// <summary>
	/// ���݃V�[��
	/// </summary>
	EnEventScene m_sceneNow = EnEventScene::en_Scene_None;
	/////////////////////////////////////////�����ݒ�n��
	int GetSceneUseWayPoint(const EnEventScene setScene)
	{
		return m_scene[setScene].m_useWayPoint;
	}
	/// <summary>
	/// �V�[���Z�b�g
	/// </summary>
	/// <param name="setScene"></param>
	void SetSceneNow(const EnEventScene setScene)
	{
		m_sceneNow = setScene;
		return;
	}
	/// <summary>
	/// �V�[�������ď����ʒu���Z�b�g
	/// </summary>
	/// <param name="setScene"></param>
	/// <param name="pos"></param>
	void SetScenePosInstance(const EnEventScene setScene
		,const Vector3 (&setVector)[5])
	{
		m_scene[setScene].m_useWayPoint = sizeof(Vector3) / sizeof((&setVector)[5]);

		int useNum = GetSceneUseWayPoint(setScene);
		
		for (int i = 0; i < 10; i++)
		{
			m_scene[setScene].m_cameraWayPoint[i] = setVector[i];
		}
	}
	/// <summary>
	/// �V�[�������ď����^�[�Q�b�g���Z�b�g
	/// </summary>
	/// <param name="setScene"></param>
	/// <param name="pos"></param>
	void SetSceneTargetInstance(const EnEventScene setScene
		,const Vector3& setVector)
	{

	}
};

