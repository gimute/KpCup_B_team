#pragma once
class Game;
class Enemy;
class Bullet;
class Player : public IGameObject
{
public:
	//�A�j���[�V�����N���b�v
	enum EnAnimationClip {
		enAnimationClip_Walk,			//����
		enAnimationClip_Idle,			//�_����
		enAnimationClip_Crouching,      //���Ⴊ��
		enAnimationClip_Punch,			//�p���`���[�V����
		enAnimationClip_Gunshot,		//�ˌ����[�V����
		enAnimationClip_Num				//�A�j���[�V������
	};
	//�v���C���[�X�e�[�g
	enum EnPlayerState {
		enPlayerState_Idle,				//�ҋ@�B
		enPlayerState_Walk,				//�����B
		enPlayerState_PostureWalk,		//�\�������B
		enPlayerState_Attack,			//�U��
	};

	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void PlayAnimation();
	void Rotation();
	void AttackRotation();
	bool AngleCheck(const Vector3& position);
	void Move();
	void ManageState();					//�X�e�[�g�J�ڏ���
	//void PlayAnimation();
	void ProcessCommonStateTransition();	//���ʏ���
	void ProcessIdleStateTransition();		//�ҋ@�J��
	void ProcessWalkStateTransition();		//�����J��
	void ProcessAttackStateTransition();    //�U���J��
	//���W�擾
	const Vector3& GetPosition() const
	{
		return m_position;
	}

//�����o�ϐ�
	ModelRender m_modelRender;
	Vector3 m_position;										//���W
	CharacterController m_charaCon;							//�L�����N�^�[�R���g���[���[
	Vector3 m_moveSpeed;									//�ړ����x�B
	Quaternion m_rotation;									//�N�H�[�^�j�I���B
	Player* m_player = nullptr;
	AnimationClip m_animationclips[enAnimationClip_Num];	//�A�j���[�V�����N���b�v
	EnPlayerState m_playerstate = enPlayerState_Idle;
	Bullet* m_bullet = nullptr;
	Vector3 m_forward = Vector3::AxisZ;						//�v���C���[�̈ړ��p���ʃx�N�g���B
	Vector3 m_up = Vector3::AxisY;
	int m_speed = 0;
	Game* m_game = nullptr;									//�Q�[��
	SphereCollider m_sphereCollider;						//�R���_�[
};

