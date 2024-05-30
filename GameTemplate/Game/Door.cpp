#include "stdafx.h"
#include "Door.h"
#include "Player.h"

Door::Door()
{

}

Door::~Door()
{

}

namespace {
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };
	const Vector3 colScale = { 135.0f,125.0f,135.0f };
	const float AddDoor = 1.5f;
}

bool Door::Start()
{
	//�h�A�g
	m_RenderDoorMain.Init("Assets/modelData/door/doorframe.tkm");
	//�ʒu�ȂǏ����ݒ�
	SetDoorMain(m_DoorMainPos, m_DoorMainRot, m_DoorMainSca);
	//�h�A�g�̓����蔻����쐬
	m_DoorMainPhy.CreateFromModel(m_RenderDoorMain.GetModel(), m_RenderDoorMain.GetModel().GetWorldMatrix());
	//�h�AL
	m_RenderDoorL.Init("Assets/modelData/door/doorLR.tkm");
	//�ʒu�ȂǏ����ݒ�
	SetDoorL(m_DoorMainPos, m_DoorMainRot, m_DoorMainSca);
	//�h�A���̓����蔻����쐬
	m_DoorLPhy.CreateFromModel(m_RenderDoorL.GetModel(), m_RenderDoorL.GetModel().GetWorldMatrix());
	//�h�AR
	m_RenderDoorR.Init("Assets/modelData/door/doorLR.tkm");
	//�ʒu�ȂǏ����ݒ�
	SetDoorR(m_DoorMainPos, m_DoorMainRot, m_DoorMainSca);
	//�h�A�E�̓����蔻����쐬
	m_DoorRPhy.CreateFromModel(m_RenderDoorR.GetModel(), m_RenderDoorR.GetModel().GetWorldMatrix());

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����B
	m_collisionObject->CreateBox(m_DoorMainPos, m_DoorMainRot, colScale);
	m_collisionObject->SetName("door_col");
	m_collisionObject->SetPosition(m_DoorMainPos + corre1);
	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<Player>("player");

	return true;
}

void Door::Update()
{
	//�R���W�����̓����蔻�菈��
	Collision();
	//�J��
	OpenClose();
	// �����蔻��̍��W���X�V
	m_DoorMainPhy.GetRigitBody()->SetPositionAndRotation(m_DoorMainPos, m_DoorMainRot);
	m_DoorLPhy.GetRigitBody()->SetPositionAndRotation(m_DoorLPos, m_DoorLRot);
	m_DoorRPhy.GetRigitBody()->SetPositionAndRotation(m_DoorRPos, m_DoorRRot);
	//�`�揈��
	m_RenderDoorMain.Update();
	m_RenderDoorL.Update();
	m_RenderDoorR.Update();
}

void Door::Collision()
{
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_col");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_collisionObject))
		{
			//m_DoorOpen = true;
			return;
		}
	}
}

void Door::OpenClose()
{
	if (!m_DoorOpen)
		return;

	//�Œᑬ�x�ɏ���������Ă���������
	Vector3 MovePowL = m_DoorLMove[1] - m_DoorLPos;
	MovePowL /= 70;
	float ratioL = m_DoorLMove[1].Length() / m_DoorLPos.Length();
	MovePowL = MovePowL * ratioL;
	Vector3 MovePowR = m_DoorRMove[1] - m_DoorRPos;
	MovePowR /= 70;
	float ratioR = m_DoorRMove[1].Length() / m_DoorRPos.Length();
	MovePowR = MovePowR * ratioR;
	if (MovePowL.Length() < 0.3f) {
		float test = MovePowL.Length();
	}

	if (m_DoorLPos.Length() < m_DoorLMove[1].Length())
	{
		m_DoorLPos += MovePowL;
		m_DoorRPos += MovePowR;
	}
	m_RenderDoorL.SetPosition(m_DoorLPos);
	m_RenderDoorR.SetPosition(m_DoorRPos);
}

void Door::Render(RenderContext& rc)
{
	m_RenderDoorMain.Draw(rc);
	m_RenderDoorL.Draw(rc);
	m_RenderDoorR.Draw(rc);
}