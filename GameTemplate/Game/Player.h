#pragma once
class Game;
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
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Rotation();
	void Move();
	//void PlayAnimation();


//�����o�ϐ�
	ModelRender m_modelRender;
	Vector3 m_position;				//���W
	CharacterController m_charaCon;	//�L�����N�^�[�R���g���[���[
	Vector3 m_moveSpeed;			//�ړ����x�B
	Quaternion m_rotation;	//�N�H�[�^�j�I���B
	Player* m_player = nullptr;
	AnimationClip m_animationclips[enAnimationClip_Num];	//�A�j���[�V�����N���b�v

	Vector3 m_moveforward = Vector3::AxisZ;		//�v���C���[�̈ړ��p���ʃx�N�g���B
	int m_speed = 0;

};

