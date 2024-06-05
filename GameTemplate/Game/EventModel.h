#pragma once

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
		AnimationClip* m_animationclips;
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
		SceneModel(int listNum,const char* tkmfilePath)
		{
			//int�^�̈����Ŕz��𓮓I�Ɋm��
			m_animationclips = new AnimationClip[listNum];

			//�����̃t�@�C���p�X��o�^
			m_sceneFilePath = tkmfilePath;
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

	/////////////////////////////////////////�����ݒ�n��
};

