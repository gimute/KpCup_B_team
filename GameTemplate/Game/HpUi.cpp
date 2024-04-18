#include "stdafx.h"
#include "HpUi.h"
///
#define DecreaseTransitionIN0	m_decrease_TRAN = en_Standby_TRAN;
#define DecreaseTransitionIN1	m_decrease_TRAN = en_DelayTimeON_TRAN
#define DecreaseTransitionIN2	m_decrease_TRAN = en_DelayTimeLoop_TRAN;
#define DecreaseTransitionIN3	m_decrease_TRAN = en_TransparentDecreaseCalc_TRAN;
#define DecreaseTransitionIN4	m_decrease_TRAN = en_TransparentDecreaseON_TRAN;
#define DecreaseTransition0 m_decrease_TRAN == en_Standby_TRAN
#define DecreaseTransition1 m_decrease_TRAN == en_DelayTimeON_TRAN
#define DecreaseTransition2	m_decrease_TRAN == en_DelayTimeLoop_TRAN
#define DecreaseTransition3	m_decrease_TRAN == en_TransparentDecreaseCalc_TRAN
#define DecreaseTransition4	m_decrease_TRAN == en_TransparentDecreaseON_TRAN
/// </summary>

namespace {
	//A�̈ʒu
	const Vector3 n_position_A{ -440.0f,350.0f,0.0f };
	//B��C�̈ʒu
	const Vector3 n_position_BC{ -765.0f,350.0f,0.0f };
	//B��C�̊�_
	const Vector2 n_pivot_BC{ 0,0.5 };
	//delaytimer�̏����ݒ莞��
	float m_delaytime = 0.7f;
}

bool HpUi::Start()
{
	//�摜�ݒ�
	m_hpUI_A.Init("Assets/modelData/ui_hp/UI_A.DDS", 670.0f, 90.0f);
	m_hpUI_B.Init("Assets/modelData/ui_hp/UI_B.DDS", 630.0f, 60.0f);
	m_hpUI_C.Init("Assets/modelData/ui_hp/UI_C.DDS", 630.0f, 60.0f);
	//�ʒu�ݒ�
	m_hpUI_A.SetPosition(n_position_A);
	m_hpUI_B.SetPosition(n_position_BC);
	m_hpUI_C.SetPosition(n_position_BC);
	//��_�ݒ�
	m_hpUI_B.SetPivot(n_pivot_BC);
	m_hpUI_C.SetPivot(n_pivot_BC);
	return true;
}

void HpUi::Update()
{
	//���Z�v�Z����
	Adjustment();
	//���ߌ��Z�v�Z����
	AdjustmentTransparent();
	//�`�揈��
	m_hpUI_A.Update();
	m_hpUI_B.Update();
	m_hpUI_C.Update();
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
		Vector3 changeS_B = m_scale_B;
		changeS_B.x = m_scale_B.x * ratio;
		m_hpUI_B.SetScale(changeS_B);
		//�ߋ�HP�Ɍ��݂�HP��ۑ�
		m_oldhp = m_nowhp;
		//���ߒx���J�n
		DecreaseTransitionIN1;
	}
}

void HpUi::AdjustmentTransparent()
{
	//�X�^���o�C��Ԃŏ��������s���Ȃ�
	if (DecreaseTransition0)
		return;

	//
	if (DecreaseTransition3 || DecreaseTransition4)
	{
		if (DecreaseTransition3)
		{
			float a = m_nowhp;
			float b = m_fullhp;
			//����HP�ƍő�HP�̊������v�Z
			float ratio = a / b;
			//���݂�m_hpUI_B��scale.x�Ɗ������v�Z
			m_changeS_C.x = m_scale_C.x * ratio;
			//���������Ɉڍs
			DecreaseTransitionIN4;
		}

		//���������A�J��Ԃ������Ōv�Z���������ƌ��݂�Scale.x�Ɠ��l�ɂȂ�܂Ō��Z
		if (DecreaseTransition4)
		{
			//�߂��Ⴍ����ׂ������炷
			m_nowScale.x -= 0.009;
			//0�ȉ���������0�ɖ߂�
			if (m_nowScale.x < 0)
			{
				m_nowScale.x = 0;
			}
			//�傫����ύX����
			m_hpUI_C.SetScale(m_nowScale);

			//����scale���ύX��scale�ȉ��ɂȂ�����
			if (m_changeS_C.x > m_nowScale.x)
			{
				//�O�̂��ߏ�����
				m_changeS_C = m_scale_C;
				m_nowScale = m_hpUI_C.GetScale();
				//�X�^���o�C�Ɉڍs
				DecreaseTransitionIN0;
			}
			//�ύX��ȏ�̏ꍇ
			else
				return;
		}
	}

	//�������牺�̏����͌����x���̃^�C�}�[����

	//delaytimer��������
	if (0.0 < m_delaytimer && DecreaseTransition2)
	{
		m_delaytimer -= g_gameTime->GetFrameDeltaTime();
	}
	//delaytimer��0�ɂȂ�����
	else if (m_delaytimer < 0.0 && DecreaseTransition2)
	{
		//����UI���Z�����v�Z�����Ɉڍs
		DecreaseTransitionIN3;
		return;
	}

	//m_delayloop��true�̎��͂��������͏������Ȃ�
	if (DecreaseTransition2)
		return;

	//���ߌ����x���^�C�}�[������
	m_delaytimer = m_delaytime;
	//�x�������Ɉڍs
	DecreaseTransitionIN2;
}

void HpUi::Render(RenderContext& rc)
{
	//HPUI�̕`�揈������
	m_hpUI_A.Draw(rc);
	//�o�[�̕`�揈�������
	m_hpUI_B.Draw(rc);
	//���߃o�[�̕`�揈������Ԍ��
	m_hpUI_C.Draw(rc);
}