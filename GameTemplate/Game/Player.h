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
		enAnimationClip_PostureWalk,	//�\������
		enAnimationClip_PostureIdle,	//�\������
		enAnimationClip_Rolling,		//���[�����O
		enAnimationClip_Num				//�A�j���[�V������
	};
	//�v���C���[�X�e�[�g
	enum EnPlayerState {
		enPlayerState_Idle,				//�ҋ@�B
		enPlayerState_Walk,				//�����B
		enPlayerState_PostureWalk,		//�\�������B
		enPlayerState_Attack,			//�U��
		enPlayerState_Rolling,
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
	void Rolling();						//�������
	void ManageState();					//�X�e�[�g�J�ڏ���
	//�A�j���[�V�����C�x���g
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//void PlayAnimation();
	void ProcessCommonStateTransition();	//���ʏ���
	void ProcessIdleStateTransition();		//�ҋ@�J��
	void ProcessWalkStateTransition();		//�����J��
	void ProcessAttackStateTransition();    //�U���J��
	void ProcessRollingStateTransition();	//���J��
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
	bool shot= false;
	Vector3 m_rollingVec = Vector3::Zero;					//������
	Vector3 m_rollingSpeed = Vector3::Zero;					//����X�s�[�h
	float m_rollingCoolDown = 0.0f;							//����N�[���_�E��
	float rollingCoolDownTime = 0.5;						//����N�[���_�E������
};

