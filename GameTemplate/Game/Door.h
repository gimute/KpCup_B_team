#pragma once
class Player;
class Door : public IGameObject
{
public:
	/////////////////////////////////////////�֐�
	Door();
	~Door();
	//�X�^�[�g�֐�
	bool Start();
	//�A�b�v�f�[�g
	void Update();
	//�����蔻�菈
	void Collision();
	//�J����
	void OpenClose();
	//�`��֐�
	void Render(RenderContext& rc);
	/////////////////////////////////////////�����o�ϐ�
	//�h�A�g
	ModelRender m_RenderDoorMain;
	//�E�h�A
	ModelRender m_RenderDoorR;
	//���h�A
	ModelRender m_RenderDoorL;
	//�v���C���[
	Player* m_player = nullptr;
	//�R���W�����I�u�W�F�N�g
	CollisionObject* m_collisionObject;
	//�h�A�g�����蔻��
	PhysicsStaticObject m_DoorMainPhy;
	//�h�A�E�����蔻��
	PhysicsStaticObject m_DoorRPhy;
	//�h�A�������蔻��
	PhysicsStaticObject m_DoorLPhy;
	/////////////////////////////////////////�ϐ�
	//�h�A�gpos
	Vector3 m_DoorMainPos = Vector3::Zero;
	//�h�A�Epos
	Vector3 m_DoorRPos = Vector3::Zero;
	//�h�A��pos
	Vector3 m_DoorLPos = Vector3::Zero;
	//�h�A�grot
	Quaternion m_DoorMainRot;
	//�h�A�Erot
	Quaternion m_DoorRRot;
	//�h�A��rot
	Quaternion m_DoorLRot;
	//�h�A�gsca
	Vector3 m_DoorMainSca = { 1.0,1.0,1.0 };
	//�h�A�g���x�N�g��
	Vector3 m_DoorMainLeft = Vector3::AxisX * -1.0f;
	//�h�A�g�E�x�N�g��
	Vector3 m_DoorMainRight = Vector3::AxisX;
	//�h�A�i���o�[
	int m_DoorNum = -1;
	//�h�A���ړ��x�N�g���z��
	Vector3 m_DoorLMove[2];
	//�h�A�E�ړ��x�N�g���z��
	Vector3 m_DoorRMove[2];
	//�h�A�J�t���O
	bool m_DoorOpen = false;
	/////////////////////////////////////////�����ݒ�n��
	//�ʒu�ݒ�
	void SetPosition(const Vector3& position)
	{
		m_DoorMainPos = position;
		return;
	}
	//��]�ݒ�
	void SetRotation(const Quaternion& rotation)
	{
		m_DoorMainRot = rotation;
		return;
	}
	//�傫���ݒ�
	void SetScale(const Vector3& scale)
	{
		m_DoorMainSca = scale;
		return;
	}
	//�h�A�i���o�[�ݒ�
	void SetDoorNumber(const int num)
	{
		m_DoorNum = num;
		return;
	}
	void SetDoorMain(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
	{
		m_RenderDoorMain.SetPosition(position);
		m_RenderDoorMain.SetRotation(rotation);
		m_RenderDoorMain.SetScale(scale);
	}
	void SetDoorL(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
	{
		m_DoorMainLeft = Vector3::AxisX * -1.0f;
		m_DoorMainRot.Apply(m_DoorMainLeft);
		m_DoorLRot = rotation;
		m_RenderDoorL.SetRotation(m_DoorLRot);
		m_DoorLPos = m_DoorMainLeft;
		m_DoorLPos.Normalize();
		m_DoorLPos *= 30.0f;
		m_DoorLPos += position;
		m_RenderDoorL.SetPosition(m_DoorLPos);
		m_RenderDoorL.SetScale(scale);
		//�ړ��z��̐ݒ�
		//�ړ��O�A���_
		m_DoorLMove[0] = m_DoorLPos;
		//�ړ���
		Vector3 Left = Vector3::AxisX * -1.0f;
		m_DoorMainRot.Apply(Left);
		Left.Normalize();
		Left *= 60.0f;
		Left += m_DoorLMove[0];
		m_DoorLMove[1] = Left;
		return;
	}
	void SetDoorR(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
	{
		m_DoorMainRight = Vector3::AxisX;
		m_DoorMainRot.Apply(m_DoorMainRight);
		m_DoorRRot = rotation;
		m_RenderDoorR.SetRotation(m_DoorRRot);
		m_DoorRPos = m_DoorMainRight;
		m_DoorRPos.Normalize();
		m_DoorRPos *= 30.0f;
		m_DoorRPos += position;
		m_RenderDoorR.SetPosition(m_DoorRPos);
		m_RenderDoorR.SetScale(scale);
		//�ړ��z��̐ݒ�
		//�ړ��O�A���_
		m_DoorRMove[0] = m_DoorRPos;
		//�ړ���
		Vector3 Right = Vector3::AxisX;
		m_DoorMainRot.Apply(Right);
		Right.Normalize();
		Right *= 60.0f;
		Right += m_DoorRMove[0];
		m_DoorRMove[1] = Right;
		return;
	}
};

