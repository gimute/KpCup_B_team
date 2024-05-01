#include "stdafx.h"
#include "Bullet.h"

//��鎞�͂���Ȋ����Ł�
//if (g_pad[0]->IsTrigger(enButtonB))
//{
//	m_bullet = NewGO<Bullet>(0, "bullet");
//	m_bullet->SetMoveDirection(m_forward);
//	m_bullet->Setrotation(m_rotation);
//	m_bullet->SetPosition(m_position);
//}

namespace {
	//�e�ۏ����f�B���C�^�C�}�[
	const float deletetimer = 0.1f;
	//�傫��
	const Vector3 scale = { 1.5f,1.5f,1.5f };
	//�ʒu�C��
	const Vector3 corre = { 0.0f,40.0f,0.0f };
}

bool Bullet::Start()
{
	m_modelrender.Init("Assets/modelData/bullet/bullet_proto.tkm");

	m_rotation.Apply(m_velocity);
	m_position += m_velocity * 50.0f;
	m_velocity *= 1200.0f;
	m_position += corre;

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����B
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 20.0f * m_scale.z);
	m_collisionObject->SetName("player_attack");
	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);

	return true;
}

void Bullet::Update()
{
	//�e�ۈړ�
	Movebullet();
	//��]����
	Rotation();
	//�e�ێ��ԏ��ŏ���
	Inpacttime();
	//�e�ۑΏۏՓˏ���
	Inpacthit();
	//�e�ۏ�������
	deletebullet();
	//�`�揈��
	m_modelrender.Update();
}

void Bullet::Movebullet()
{
	//���W���ړ�������B
	m_position += m_velocity * g_gameTime->GetFrameDeltaTime();
	m_modelrender.SetPosition(m_position);
	m_collisionObject->SetPosition(m_position);
	bullettime -= g_gameTime->GetFrameDeltaTime();	//���R�����^�C�}�[�����炷���c
}

void Bullet::Rotation()
{
	//���m�ɂ͒e�ۂ��ړ������Ɍ�������v���O����
				//�ړ����x�����ɓ����Ƃł���B
	float angle = atan2(-m_velocity.x, m_velocity.z);
	m_rotation.SetRotationY(-angle);
	m_modelrender.SetRotation(m_rotation);
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Bullet::Inpacttime()
{
	if (bullettime > 0)
		return;
	DeleteGO(m_collisionObject);
	DeleteGO(this);
}

void Bullet::Inpacthit()
{
	//enemy�̃R���W�������擾����												//��enemy�̋��ʃR���W����
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_col");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_collisionObject))
		{
			if (m_isDelete == false) {
				//deletebullet��if�����ʂ�悤�ɂ���
				m_isDelete = true;
				//deletetimer�͌���0.2f
				m_deleteTimer = deletetimer;
			}
		}
	}
}

void Bullet::deletebullet()
{
	if (m_isDelete)
	{

		m_deleteTimer -= g_gameTime->GetFrameDeltaTime(); //deletetimer��1�t���[������
		//���炷�B
			//���^�C�}�[���[���ɂȂ�����B(deletetimer���0�̕����傫���Ȃ�����)
		if (m_deleteTimer <= 0.0f)
		{
			//��������
			DeleteGO(m_collisionObject);
			DeleteGO(this);
		}
	}
}

void Bullet::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
}
