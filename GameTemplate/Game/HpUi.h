#pragma once
class HpUi : public IGameObject
{

public:
	/////////////////////////////////////////�֐�
	HpUi(){}
	~HpUi(){}
	//�X�^�[�g�֐�
	bool Start();
	//�A�b�v�f�[�g
	void Update();
	//�����v�Z
	void Adjustment();
	//�����_�[
	void Render(RenderContext& rc);
	/////////////////////////////////////////�����o�ϐ�
	//HPUI
	SpriteRender m_hpUI_A;
	//HP�o�[
	SpriteRender m_hpUI_B;
	/////////////////////////////////////////�ϐ�
	//�ő�HP
	int m_fullhp = 100;
	//����HP�@
	int m_nowhp = m_fullhp;
	//�ߋ�HP
	int m_oldhp = m_fullhp;
	//����hp�o�[�X�P�[��(�������̂�m_hpUI_B�Az�͌Œ�)
	Vector3 m_scale_B = { 1.0,1.0,1.0 };
	/////////////////////////////////////////�����ݒ�n��
	//HP��������
	void DecreaseHP(int decrease)
	{
		//HP�������l���炷
		m_nowhp -= decrease;
		return;
	}
};

