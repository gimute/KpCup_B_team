#pragma once
class Game;
class Player;
class Enemy : public IGameObject
{
public:
	//�s���X�e�[�g
	enum EnEnemyState {
		enEnemyState_Idle,			//�ҋ@
		enEnemyState_Chase,			//�ǐ�
		enEnemyState_Attack,		//�U��
		//enEnemyState_ReceiveDamage,	//��_���[�W
		//enEnemyState_Down,			//�_�E��
	};
	//�A�j���[�V������/////////////////////////////
//�A�j���[�V�����X�e�[�g
	enum EnAnimationClip {
		enAnimationClip_Idle,			//�_����
		enAnimationClip_Chase,			//�ǐ�
		enAnimationClip_Attack,			//�U��
		//enAnimationClip_ReceiveDamage,	//��_���[�W
		//enAnimationClip_Down,			//�_�E��
		enAnimationClip_Num,			//�A�j���[�V������
	};
	void PlayAnimation();
	//�X�e�[�g�J�ڊ֐�///////////////////////////////////////////
	void ManageState();							//�J�ڏ���
	void ProcessCommonStateTransition();		//���ʑJ�ڏ���
	void ProcessIdleStateTransition();			//�ҋ@�J��
	void ProcessChaseStateTransition();			//�ǐՑJ��
	void ProcessAttackStateTransition();		//�U���J��
	void ProcessReceiveDamageStateTransition();	//��_���J��
	void ProcessDownStateTransition();			//�_�E���J��
	/////////////////////////////////////////////////////////////
	Enemy() {};
	~Enemy() {};
	bool Start();						//�A�b�v�f�[�g
	void Update();                                         //�A�b�v�f�[�g
	//���f���̉�]������֐�
	void Rotation();                                       //��]
	void Chase();										   //�ǐ�
	void Attack();										   //�U��
	//void Collision();										//�{�̂̓����蔻��
	
	//�v���C���[�����E���ɋ��邩�m���߂�֐�
	const bool SearchPlayer() const;

	//�v���C���[�����͈͓��ɋ��邩�m���߂�֐�
	const bool SearchChaseDistance() const;

	//�v���C���[���U���͈͓��ɋ��邩�m���߂�֐�
	const bool SearchAttackDistance() const;

	void Render(RenderContext& rc);
	///////////////////////////////////////////////////////////
	//�����ݒ�n��
	//�����o�֐��錾
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	Game* m_game = nullptr;
	AnimationClip m_animationclips[enAnimationClip_Num];     //�A�j���[�V�����N���b�v
	EnEnemyState m_enemystate = enEnemyState_Idle;          //�G�l�~�[�X�e�[�g
	Vector3	m_forward = Vector3::AxisZ;						//�G�l�~�[�̐��ʃx�N�g���B
	Vector3 m_movespeed;									//�ړ����x
	Vector3 m_position;										//���W
	Quaternion m_rotation;					//��]
	Vector3 m_scale = Vector3::One;			//�傫��
	ModelRender m_modelRender;	//���f�������_�[
	CharacterController m_charaCon;        //�L�����R��
};

