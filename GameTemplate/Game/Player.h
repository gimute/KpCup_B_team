#pragma once
class Game;
class Player
{
public:
	Player();
	~Player();
	bool Start();
	void Updata();
	void Render(RenderContext& rc);

	void Rotation();
	void Move();
	void PlayAnimation();



	ModelRender m_modelRender;
	Vector3 m_position;				//���W
	CharacterController m_charaCon;	//�L�����N�^�[�R���g���[���[
	Vector3 m_moveSpeed;			//�ړ����x�B
	Quaternion m_rotation;	//�N�H�[�^�j�I���B
	Player* m_player = nullptr;


};

