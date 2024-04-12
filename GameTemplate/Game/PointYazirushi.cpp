#include "stdafx.h"
#include "PointYazirushi.h"
#include "Player.h"

namespace {
	const Vector3 correction = { 0.0,250.0,0.0 };
}

bool PointYazirushi::Start()
{
	//���f����ݒ�
	m_modelrender.Init("Assets/modelData/yazirushi/yazirushi.tkm");
	//�ʒu�ݒ�
	m_modelrender.SetPosition(m_position);
	//��]�ݒ�
	m_modelrender.SetRotation(m_rotation);
	//�傫���ݒ�
	m_modelrender.SetScale(m_scale);
	//�v���C���[�ǂݍ���
	m_player = FindGO<Player>("player");
	//�ʒu�X�V�J�n
	m_positionupdate = true;
	return true;
}

void PointYazirushi::Update()
{
	//�ʒu�X�V
	PositionUpdate();
	//��]����
	Rotation();
	//�h���[�R�[��
	m_modelrender.Update();
}

void PointYazirushi::PositionUpdate()
{
	if (m_positionupdate == true)
	{
		//�ʒu�������グ��
		Vector3 pos = correction + m_player->m_position;
		//�ʒu�X�V
		m_modelrender.SetPosition(pos);
	}
}

void PointYazirushi::Rotation()
{
	if (m_positionupdate == true)
	{
		//�₶�Ώۍ��W-�v���C���[���W���v�Z���āA�v���C���[���W����₶�Ώۂւ̃x�N�g�����o��
		Vector3 diff = m_targetpos - m_player->m_position;
		//�����������~�����̂ŒP�ʃx�N�g����
		diff.Normalize();
		//�X�V�Ȃ�
		m_rotation.SetRotation(m_forward, diff);
		m_modelrender.SetRotation(m_rotation);
	}
}

void PointYazirushi::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
}