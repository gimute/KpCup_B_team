#pragma once
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
class HpUi : public IGameObject
{

public:
	//����UI�����J��
	enum DecreaseTransition {
		//�ҋ@0
		en_Standby_TRAN,
		//�x���J�n1
		en_DelayTimeON_TRAN,
		//�x������2
		en_DelayTimeLoop_TRAN,
		//����UI�����v�Z3
		en_TransparentDecreaseCalc_TRAN,
		//����UI�����J�n4
		en_TransparentDecreaseON_TRAN
	};
	/////////////////////////////////////////�֐�
	HpUi() {}
	~HpUi() {}
	//�X�^�[�g�֐�
	bool Start();
	//�A�b�v�f�[�g
	void Update();
	//�����v�Z
	void Adjustment();
	//���߃o�[�����v�Z
	void AdjustmentTransparent();
	//�����_�[
	void Render(RenderContext& rc);
	/////////////////////////////////////////�����o�ϐ�
	//HPUI
	SpriteRender m_hpUI_A;
	//HP�o�[
	SpriteRender m_hpUI_B;
	//����HP�o�[
	SpriteRender m_hpUI_C;
	//����UI�����J��
	DecreaseTransition m_decrease_TRAN = en_Standby_TRAN;
	/////////////////////////////////////////�ϐ�
	//�ő�HP
	int m_fullhp = 100;
	//����HP�@
	int m_nowhp = m_fullhp;
	//�ߋ�HP
	int m_oldhp = m_fullhp;
	//����hp�o�[�X�P�[��(�������̂�m_hpUI_B�Az�͌Œ�)
	Vector3 m_scale_B = { 1.0,1.0,1.0 };
	//���ݓ���hp�o�[�X�P�[��(�������̂�m_hpUI_C�Az�͌Œ�)
	Vector3 m_scale_C = { 1.0,1.0,1.0 };
	//�����x���^�C�}�[
	float m_delaytimer = 0.0;
	//���ߌ�������
	Vector3 m_changeS_C = m_scale_C;
	//���ݓ��ߌ����l
	Vector3 m_nowScale = m_hpUI_C.GetScale();
	/////////////////////////////////////////�����ݒ�n��
	//HP��������
	void DecreaseHP(int decrease)
	{
		//HP�������l���炷
		m_nowhp -= decrease;
		return;
	}
};

