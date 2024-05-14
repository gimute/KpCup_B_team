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
	/// ���f�������_�[
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/////////////////////////////////////////�����o�ϐ�
	/// <summary>
	/// �V�O�i���X�v���C�g�����_�[
	/// </summary>
	SpriteRender m_SignalUi;
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
	/////////////////////////////////////////�����ݒ�n��
	void SetEnemyVecNum(int num)
	{
		m_enemyConnectNum = num;
		return;
	}
};

