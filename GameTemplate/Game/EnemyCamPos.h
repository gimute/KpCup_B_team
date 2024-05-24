#pragma once

#define LIST_MAX 3

class GameCamera;
class Enemy;
class Game;
class Player;
class EnemyCamPos
{
public:
	/// <summary>
	/// �J�������ʂɂ���G�l�~�[���i�[���邽�߂̍\����
	/// </summary>
	struct EnemyCamForward 
	{
		/// <summary>
		/// ���X�g�ԍ�
		/// </summary>
		int m_num = -1;
		/// <summary>
		/// �o�^����EnemyList�̔ԍ�
		/// </summary>
		int m_registNum = -1;
		/// <summary>
		/// ���X�g�ɐݒ蒆�ł��邩�ǂ���
		/// </summary>
		bool m_enemyRegist = false;
	};
	/////////////////////////////////////////�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	EnemyCamPos();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~EnemyCamPos();
	/// <summary>
	/// �J�����ƃG�l�~�[�̈ʒu���m�F��
	/// �ʒu�Ƌ����ɂ����Enemy�̍U���p�x��
	/// �ύX����֐�
	/// </summary>
	float EnemyCamPosConfirmation(Enemy* enemy);
	/// <summary>
	/// ���ʂ�Enemy�����݂��邩���肷��֐�
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	bool AngleCheck(const Vector3& position);
	/// <summary>
	/// m_camForwardList�ɋ�̗v�f����������o�^��
	/// �o�^����Enemy�̍U���p�x��"��"����֐�
	/// </summary>
	/// <param name="ListNum"></param>
	void Registration(int ListNum);
	/// <summary>
	/// m_camForwardList�ɋ�̗v�f���Ȃ��ꍇ�ɂ���Enemy��
	/// ���ݓo�^����Enemy�̒��ň�ԃJ�������牓��Enemy����
	/// �߂�����m_camForwardLis�ɓo�^���U���p�x��"��"����֐�
	/// </summary>
	/// <param name="ListNum"></param>
	/// <param name="enemypos"></param>
	void CompareNear(int ListNum,const Vector3& enemypos);
	/// <summary>
	/// �J�����͈͓��ɂ��Ȃ�Enemy�̍U���p�x��"��"�ɂ���
	/// �܂��̓J�����͈͓��ɂ��炸�Am_camForwardList�ɓo�^����
	/// �v�f�𖢓o�^��Ԃɂ�Enemy�̍U���p�x��"��"�ɂ���֐�
	/// </summary>
	/// <param name="ListNum"></param>
	void CamOut(int ListNum,const Vector3 enemypos);
	/// <summary>
	///	m_camForwardList�ɋ󂫂����邩�ǂ����𒲂ׂ�֐�
	/// </summary>
	bool ListCheck();
	/// <summary>
	/// m_camForwardList�ɓ����v�f�ԍ������邩�ǂ����𒲂ׂ�֐�
	/// </summary>
	/// <param name="ListNum"></param>
	/// <returns></returns>
	bool ListCheck(int ListNum);
	/// <summary>
	/// ���ݒT������Enemy�̃X�e�[�g��Attack�X�e�[�g���ǂ������ׂ�
	/// </summary>
	/// <param name="ListNum"></param>
	/// <returns></returns>
	bool EnemyStateIsAttack(int ListNum);
	/////////////////////////////////////////�����o�ϐ�
	/// <summary>
	/// �Q�[��
	/// </summary>
	Game* m_game = nullptr;
	/// <summary>
	/// �Q�[���J����
	/// </summary>
	GameCamera* m_camera = nullptr;
	/// <summary>
	/// �v���C���[
	/// </summary>
	Player* m_player = nullptr;
	/////////////////////////////////////////�ϐ�
	/// <summary>
	/// �J�������ʃG�l�~�[�̍\���̔z��
	/// </summary>
	EnemyCamForward m_camForwardList[LIST_MAX];
	/// <summary>
	///	���݂̋󂫗v�f
	/// </summary>
	int m_emptyPoint = 0;
	/////////////////////////////////////////�����ݒ�n��
	bool PointerNull()
	{
		if (m_game == nullptr||m_player == nullptr||m_camera == nullptr)
		{
			return true;
		}

		return false;
	}
};

