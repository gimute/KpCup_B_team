#pragma once
class GameCamera;
class SignalRailUi;
class Game;
class Player;
class SignalUi : public IGameObject
{
public:
	/////////////////////////////////////////�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SignalUi();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SignalUi();
	/// <summary>
	/// �X�^�[�g�֐�
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// �A�b�v�f�[�g
	/// </summary>
	void Update();
	/// <summary>
	/// ��]
	/// </summary>
	void Rotation();
	/// <summary>
	/// �댯�M��Ui�폜
	/// </summary>
	void DeleteSignal();
	/// <summary>
	/// �A���t�@�l�̌v�Z
	/// </summary>
	void AlphaCalc();
	/// <summary>
	/// ���f�������_�[
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/////////////////////////////////////////�����o�ϐ�
	/// <summary>
	/// ���ӃV�O�i���X�v���C�g�����_�[
	/// </summary>
	SpriteRender m_CautionSignalUi;
	/// <summary>
	/// �댯�V�O�i���X�v���C�g�����_�[
	/// </summary>
	SpriteRender m_DangerSignalUi;
	/// <summary>
	/// �Q�[���J����
	/// </summary>
	GameCamera* m_gameCamera = nullptr;
	/// <summary>
	/// �V�O�i�����[��
	/// </summary>
	SignalRailUi* m_signalRailUi = nullptr;
	/// <summary>
	/// �Q�[��
	/// </summary>
	Game* m_game = nullptr;
	/// <summary>
	/// �v���C���[
	/// </summary>
	Player* m_player = nullptr;
	/// <summary>
	/// �����e�N�X�`��
	/// </summary>
	Texture m_texture;
	/////////////////////////////////////////�ϐ�
	/// <summary>
	/// �̔z��ԍ����i�[����ϐ�
	/// </summary>
	int m_enemyConnectNum = -1;
	/// <summary>
	///	�z��ԍ�
	/// </summary>
	int m_VecNum = -1;
	/// <summary>
	/// 2D��ԗp�̏�����̃x�N�g��z��0.0�Œ�
	/// </summary>
	Vector3 m_up2D = { 0.0f,1.0f,0.0f };
	/// <summary>
	/// 2D��ԗp�̍����i�[�p�x�N�g��
	/// </summary>
	Vector3 m_diff2D = Vector3::Zero;
	/// <summary>
	/// �폜�^�C�}�[
	/// </summary>
	float m_deleteTimer = 0.0f;
	/// <summary>
	/// �V�F�[�_�[�ɑ���A���t�@�l
	/// </summary>
	float m_alpha = 0.1;
	/// <summary>
	/// �A���t�@�l�̌v�Z�Ɏg�p����Bool
	/// </summary>
	bool m_alphaCalcBool = true;
	/////////////////////////////////////////�����ݒ�n��
	/// <summary>
	/// �G�l�~�[�̗v�f�ԍ����擾
	/// </summary>
	/// <param name="num"></param>
	void SetEnemyVecNum(int num)
	{
		m_enemyConnectNum = num;
		return;
	}
};

