#pragma once
class Player;
class PointYazirushi : public IGameObject
{
public:
	/////////////////////////////////////////�֐�
	PointYazirushi() {}
	~PointYazirushi() {}
	//�X�^�[�g�֐�
	bool Start();
	//�A�b�v�f�[�g�֐�
	void Update();
	//�ʒu�X�V
	void PositionUpdate();
	//��]����
	void Rotation();
	//�����_�[
	void Render(RenderContext& rc);
	/////////////////////////////////////////�����o�ϐ�
	//���f�������_�[
	ModelRender m_modelrender;
	//���W
	Vector3 m_position;
	//�傫��
	Vector3 m_scale = Vector3::One;
	//��]
	Quaternion m_rotation;
	//���ʃx�N�g��
	Vector3	m_forward = Vector3::AxisZ;
	//�v���C���[
	Player* m_player = nullptr;
	/////////////////////////////////////////�ϐ�
	//�₶�Ώۈʒu
	Vector3 m_targetpos = { 0.0,0.0,0.0 };
	//���W�X�V�����邩
	bool m_positionupdate = false;
	/////////////////////////////////////////�����ݒ�n��
public:
	//�ʒu�ݒ�
	void SetPosition(const Vector3& position)
	{
		m_position = position;
		return;
	}
	//���^�[�Q�b�g�ݒ�
	void SetTarget(const Vector3& position)
	{
		m_targetpos = position;
		return;
	}
	//��]�ݒ�
	void SetRotation(const Quaternion& rotation) {
		m_rotation = rotation;
		return;
	}
	//�傫���ݒ�
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		return;
	}
};

