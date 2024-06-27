#pragma once
class RemainingBulletsUi : public IGameObject
{
public:
	/////////////////////////////////////////�\����
	struct RemainingBulletReloadUiWipeParam
	{
		/// <summary>
		/// �����[�h�\���X�v���C�g�̃��C�v�����ϐ�
		/// </summary>
		Vector2 m_remainingBulletReloadUiWipeDir;
		/// <summary>
		/// �����[�h�\���X�v���C�g�̃��j�A���C�v�ϐ�
		/// </summary>
		float m_remainingBulletReloadUiWipeSize = 0.0f;
		/// <summary>
		/// ���݃��C�v�T�C�Y�擾
		/// </summary>
		/// <returns></returns>
		float GetWipeSize()
		{
			return m_remainingBulletReloadUiWipeSize;
		}
	};
	/////////////////////////////////////////�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	RemainingBulletsUi();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~RemainingBulletsUi();
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
	/// �c�e���`�揈��
	/// </summary>
	void RemainingBulletNumberUiUpdate();
	/// <summary>
	/// ���C�v�v�Z
	/// </summary>
	void WipeCalc();
	/// <summary>
	/// �`��֐�
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/////////////////////////////////////////�ϐ�
	/// <summary>
	/// �c�e��
	/// </summary>
	int *m_RemainingBulletNumber;
	/// <summary>
	/// �ő�c�e��
	/// </summary>
	int m_RemainingBulletNumberFull = 0;
	/// <summary>
	/// �����[�h�\���X�v���C�g�̃��C�v�p�����[�^�[
	/// </summary>
	RemainingBulletReloadUiWipeParam m_remainingBulletReloadUiWipeParam;
	/// <summary>
	/// �����[�h��Ԃ��ǂ���
	/// </summary>
	bool m_isReload = false;
	/////////////////////////////////////////�X�v���C�g
	/// <summary>
	///	�c�e���x�[�X�X�v���C�g
	/// </summary>
	SpriteRender m_RemainingBulletUi;
	/// <summary>
	/// �����[�h�\���X�v���C�g
	/// </summary>
	SpriteRender m_RemainingBulletReloadUi;
	/// <summary>
	/// �����[�h�\���e�N�X�`��
	/// </summary>
	Texture m_RemainingBulletReloadTexture;
	/// <summary>
	/// �c�e���X�v���C�g
	/// </summary>
	FontRender m_RemainingBulletNumberUi;
	/////////////////////////////////////////�����o�֐�
	/// <summary>
	/// �c�e�A�h���X�o�^
	/// </summary>
	/// <param name="bulletNum"></param>
	void InitRemainingBulletNum(int *bulletNum)
	{
		m_RemainingBulletNumber = bulletNum;
		m_RemainingBulletNumberFull = *m_RemainingBulletNumber;
	}
	/// <summary>
	/// �����[�h��Ԃ̕ύX
	/// </summary>
	/// <param name="isReload"></param>
	void IsReload(bool isReload)
	{
		m_isReload = isReload;
	}
};

