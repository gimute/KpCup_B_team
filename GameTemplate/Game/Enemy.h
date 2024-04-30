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
	~Enemy();
	bool Start();						//�A�b�v�f�[�g
	void Update();                                         //�A�b�v�f�[�g
	void Rotation();                                       //��]
	void Chase();										   //�ǐ�
	void Attack();										   //�U��
	//void Collision();										//�{�̂̓����蔻��
	const bool SearchPlayer() const;                       //�v���C���[�T�m
	const bool SearchChaseDistance() const;					//�ǐՋ����T�m
	const bool SearchAttackDistance() const;               //�U�������T�m
	void Render(RenderContext& rc);                        //���f�������_�[
	//bool IsAttack() const				//�ړ��ł��邩�ǂ���
	//{
	//	return m_enemystate != enAnimationClip_Chase &&
	//		m_enemystate != enAnimationClip_Attack;
	//}
	///////////////////////////////////////////////////////////
	//�����ݒ�n��
	//�����o�֐��錾
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	Game* m_game = nullptr;
	//CollisionObject* m_attackcoll;		//�R���W�����I�u�W�F�N�g�B
	AnimationClip m_animationclips[enAnimationClip_Num];     //�A�j���[�V�����N���b�v
	EnEnemyState m_enemystate = enEnemyState_Idle;          //�G�l�~�[�X�e�[�g
	Vector3	m_forward = Vector3::AxisZ;						//�G�l�~�[�̐��ʃx�N�g���B
	Vector3 m_movespeed;									//�ړ����x
	Vector3 m_position;										//���W
	Quaternion m_rotation;					//��]
	Vector3 m_scale = Vector3::One;			//�傫��
	ModelRender m_modelRender;	//���f�������_�[
	//CollisionObject* m_collisionObject;		//�R���W�����I�u�W�F�N�g�B
	CharacterController m_charaCon;        //�L�����R��
	int m_hp = 0;                           //HP
	int m_sh = 0;							//�V�[���h
	int m_Vectornum = 0;					//�z��̃i���o�[
	float m_idleTimer= 0.0f;								//�ҋ@���ԃ^�C�}�[�B
	float m_chaseTimer = 0.0f;								//�ǐՎ��ԃ^�C�}�[�B
	float m_attackTimer = 5.0f;								//�U�����ԃ^�C�}�[�B
	float attacktime = 5.0f;
	float m_attackGotimer = 0.0;							//�U�����ԃ^�C�}�[
	float attackGotime = 2.0;								//�U������
	float m_attackcooltimer = 0.0f;
	float attackcooltime = 3.0f;
	float m_looptimer = 0.0f;								//���[�v�^�C�}�[
	float looptime = 1.16;									//���[�v����
	float m_mutekitimer = 0.0f;								//���G�^�C�}�[
	float mutekitime = 0.1f;								//���G����
};

