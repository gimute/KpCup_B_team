#pragma once

#include <memory>

class EventCamera;

class EventModel : public IGameObject
{
public:
	/// <summary>
	/// 1�V�[���Ŏg�p���郂�f���A�A�j���[�V�����̍\����
	/// </summary>
	struct SceneModel
	{
		/// <summary>
		/// ���̃V�[���Ŏg�p���郂�f��
		/// </summary>
		ModelRender m_modelRender;
		/// <summary>
		/// ���̃V�[���Ŏg�p����A�j���[�V����
		/// </summary>
		std::unique_ptr<AnimationClip[]> m_animationclips;
		/// <summary>
		/// ���̃V�[���Ŏg�p����A�j���[�V�����̐�
		/// </summary>
		int m_useAnimationclips = 0;
		/// <summary>
		/// ���̃V�[���Ŏg�p���郂�f���̃t�@�C���p�X
		/// </summary>
		const char* m_sceneFilePath;
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="listNum"></param>
		/// <param name="tkmfilePath"></param>
		SceneModel(int listNum,const char* tkmfilePath)
		{
			//int�^�̈����Ŕz��𓮓I�Ɋm�ہA�X�}�[�g�|�C���^�Ƀ������̏��L�����ς˂�
			m_animationclips = std::make_unique<AnimationClip[]>(listNum);

			//�����̃t�@�C���p�X��o�^
			m_sceneFilePath = tkmfilePath;
		}
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~SceneModel()
		{
			//�X�}�[�g�|�C���^�ɔC���Ă邩�獡�͉��������ĂȂ�
		}
		/// <summary>
		/// ��������int�^�̐��̃A�j���[�V�����N���b�v�̗v�f�ԍ��̏ꏊ��
		/// �������̃t�@�C���p�X��o�^���A��O������bool�^���A
		/// �A�j���[�V�����̃��[�v�t���O�Ƃ��ăA�j���[�V�����N���b�v�ɓo�^����B
		/// </summary>
		/// <param name="listNum"></param>
		/// <param name="filePath"></param>
		/// <param name="loopFlag"></param>
		void AnimationRegistration(int listNum,const char* filePath,bool loopFlag)
		{
			//������int�^�̐��l�̔z��̗v�f�ԍ��̏ꏊ�Ɉ����̃t�@�C���p�X��o�^
			m_animationclips[listNum].Load(filePath);

			//������bool�^�Ń��[�v�t���O��ݒ�
			m_animationclips[listNum].SetLoopFlag(loopFlag);
		}
		/// <summary>
		/// ���f�������_�[�ɓo�^���m��
		/// </summary>
		void RegistrationConfirmed()
		{
			//�o�^���m�肷��
			m_modelRender.Init(m_sceneFilePath, m_animationclips.get(), m_useAnimationclips);
		}
	};
public:
	/////////////////////////////////////////�֐�
	EventModel();
	~EventModel();
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
	/// <summary>
	/// ���x�������_�[
	/// </summary>
	LevelRender m_camLevelRender;
	/// <summary>
	/// �C�x���g�J�����̃C���X�^���X�擾�p�ϐ�
	/// </summary>
	EventCamera* m_eventCam = nullptr;
	/////////////////////////////////////////�ϐ�
	/// <summary>
	/// ���f���̔z��
	/// </summary>
	std::list<SceneModel> m_sceneModelMapList;
	/// <summary>
	/// �C�x���g���Đ����邩���Ȃ����̃t���O
	/// </summary>
	bool m_eventFlag = false;
	/////////////////////////////////////////�����ݒ�n��
};

