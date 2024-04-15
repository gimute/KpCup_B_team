#include "stdafx.h"
#include "HpUi.h"

namespace {
	const Vector3 n_position_A{ -600.0f,400.0f,0.0f };
	const Vector3 n_position_B{ -940.0f,400.0f,0.0f };
	const Vector2 n_pivot_B{ 0,0.5 };
}

bool HpUi::Start()
{
	//�摜�ݒ�
	m_hpUI_A.Init("Assets/modelData/ui_hp/UI_A.DDS", 700.0f, 120.0f);
	m_hpUI_B.Init("Assets/modelData/ui_hp/UI_B.DDS", 670.0f, 90.0f);
	//�ʒu�ݒ�
	m_hpUI_A.SetPosition(n_position_A);
	m_hpUI_B.SetPosition(n_position_B);
	//��_�ݒ�
	m_hpUI_B.SetPivot(n_pivot_B);
	return true;
}

void HpUi::Update()
{
	//�����v�Z����
	Adjustment();
	//�`�揈��
	m_hpUI_A.Update();
	m_hpUI_B.Update();
}

void HpUi::Adjustment()
{
	//0�ȉ��ɂ��Ȃ�
	if (m_nowhp < 0) {
		m_nowhp = 0;
	}
	//����HP������������
	if (m_nowhp < m_oldhp)
	{
		float a = m_nowhp;
		float b = m_fullhp;
		//����HP�ƍő�HP�̊������v�Z
		float ratio = a / b;
		//���݂�m_hpUI_B��scale.x�Ɗ������v�Z
		Vector3 m_changeS_B = m_scale_B;
		m_changeS_B.x = m_scale_B.x * ratio;
		m_hpUI_B.SetScale(m_changeS_B);
		//�ߋ�HP�Ɍ��݂�HP��ۑ�
		m_oldhp = m_nowhp;
	}
}

void HpUi::Render(RenderContext& rc)
{
	//HPUI�̕`�揈������
	m_hpUI_A.Draw(rc);
	//�o�[�̕`�揈�������
	m_hpUI_B.Draw(rc);
}