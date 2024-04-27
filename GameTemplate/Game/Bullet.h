#pragma once
class Bullet : public IGameObject
{
public:
	/////////////////////////////////////////�֐�
	Bullet() {};
	~Bullet() {};
	bool Start();
	//�A�b�v�f�[�g
	void Update();
	//��]
	void Rotation();
	//�e�ۈړ�
	void Movebullet();
	//���f�������_�[
	void Render(RenderContext& rc);
	//�e�ێ��R����
	void Inpacttime();
	//�e�ۏՓ�
	void Inpacthit();
	//�e�ۏ���
	void deletebullet();
	/////////////////////////////////////////�����o�ϐ�
	//���f�������_�\
	ModelRender m_modelrender;
	//�R���W�����I�u�W�F�N�g
	CollisionObject* m_collisionObject;
	/////////////////////////////////////////�ϐ�
	//���ʃx�N�g��
	Vector3 m_forward = Vector3::AxisZ;
	//���x
	Vector3 m_velocity = Vector3::AxisZ;
	//�i�s����
	Vector3 m_direction = Vector3::Zero;
	//�ʒu
	Vector3 m_position = Vector3::Zero;
	//��]
	Quaternion m_rotation;
	//�傫��
	Vector3	m_scale = Vector3::One;
	//���R�����^�C�}�[
	float bullettime = 1.5f;
	//�����f�B���C
	float m_deleteTimer = 0.0f;
	bool m_isDelete = false;
	/////////////////////////////////////////�����ݒ�n��
	//�i�s�����ݒ�(�v���C���[�̐��ʃx�N�g���Ȃ�)
	void SetMoveDirection(const Vector3 direction)
	{
		m_direction = direction;
		return;
	}
	//�ʒu�ݒ�
	void SetPosition(const Vector3 position)
	{
		m_position = position;
		return;
	}
	//��]�ݒ�
	void Setrotation(const Quaternion& rotation)//��]�l�����ݒ�
	{
		m_rotation = rotation;
	}
};

