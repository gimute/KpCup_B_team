#pragma once

#define ATTACK_POINT_NUM 15		//�A�^�b�N�|�C���g�̐�

//�G�l�~�[�̍U���|�C���g
//���̃|�C���g���m�ۂł��Ă���G�l�~�[�̂ݍU���\
class EnemyAttackPoint
{
public:

	//�A�^�b�N�|�C���g�̍\����
	struct AttackPoint
	{
		Vector3 m_position;				//�A�^�b�N�|�C���g�̍��W
		int m_number;					//�A�^�b�N�|�C���g�̗v�f�ԍ�
		bool m_use = false;				//�g�p����
		Enemy* m_useEnemy = nullptr;	//�A�^�b�N�|�C���g���g�p���Ă���G�l�~�[
	};

	EnemyAttackPoint();
	~EnemyAttackPoint();

	
	//�A�^�b�N�|�C���g�̍��W���X�V����֐�
	//�����ő���ꂽ���W����ɁA�~��Ƀ|�C���g��ݒu����
	void Update(Vector3 position);

	//�߂��ɂ���g�p�\�ȃA�^�b�N�|�C���g�̃A�h���X���擾����֐�
	AttackPoint* GetNearAttackPoint(Vector3 position);

	//�A�^�b�N�|�C���g���m�ۂ���֐�
	//�m�ۂ���A�^�b�N�|�C���g�̃i���o�[��
	//�o�^�p�ɁA�|�C���g���m�ۂ���G�l�~�[�̃A�h���X���󂯎��
	void GetAttackPoint(int number, Enemy* enemy);

	//�A�^�b�N�|�C���g���������֐�
	//�������A�b�N�|�C���g�̃i���o�[��
	//�m�F�p�ɁA�|�C���g���m�ۂ��Ă���G�l�~�[�̃A�h���X���󂯎��
	void ReleaseAttackPoint(int number, Enemy* enemy);
	

	//�A�^�b�N�|�C���g�����p�\���ǂ���
	//����A�A�^�b�N�|�C���g���g���Ă���G�l�~�[����萔����ꍇ���p�𐧌�����悤�ɂ��Ă���
	bool IsUsableAttackPoint();

private:
	AttackPoint m_AttackPointList[ATTACK_POINT_NUM];
	int m_useAttackPointNum = 0;
};

