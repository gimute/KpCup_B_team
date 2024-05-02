#pragma once
#include "Game.h"
#include "Enemy.h"
class Game;
class Enemy;
class Player;
class EnemyHpUi : public IGameObject
{
	/// <summary>
	/// ����HPUI�����J��
	/// </summary>
	enum DecreaseTransition {
		/// <summary>
		/// �ҋ@0
		/// </summary>
		en_Standby_TRAN,
		/// <summary>
		/// �x���J�n1
		/// </summary>
		en_DelayTimeON_TRAN,
		/// <summary>
		/// �x������2
		/// </summary>
		en_DelayTimeLoop_TRAN,
		/// <summary>
		/// ����UI�����v�Z3
		/// </summary>
		en_TransparentDecreaseCalc_TRAN,
		/// <summary>
		/// ����UI�����J�n4
		/// </summary>
		en_TransparentDecreaseON_TRAN
	};
	/////////////////////////////////////////�֐�
public:
	EnemyHpUi() {};
	~EnemyHpUi() {};
	/// <summary>
	/// �X�^�[�g�֐�
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// �A�b�v�f�[�g�֐�
	/// </summary>
	void Update();
	/// <summary>
	/// �����\���v�Z
	/// </summary>
	void DisplayDis();
	/// <summary>
	/// �ʒu�X�V
	/// </summary>
	void PositionUpdate();
	/// <summary>
	/// ���Z�v�Z
	/// </summary>
	void Adjustment();
	/// <summary>
	/// ���߃o�[���Z
	/// </summary>
	void AdjustmentTransparent();
	/// <summary>
	/// ���f�������_�[
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/////////////////////////////////////////�����o�ϐ�
	/// <summary>
	/// HPUI
	/// </summary>
	SpriteRender m_hpUI_A;
	/// <summary>
	/// HP�o�[
	/// </summary>
	SpriteRender m_hpUI_B;
	/// <summary>
	/// ����HP�o�[
	/// </summary>
	SpriteRender m_hpUI_C;
	/// <summary>
	/// ����HP�o�[�����J��
	/// </summary>
	DecreaseTransition m_decrease_TRAN = en_Standby_TRAN;
	/// <summary>
	/// �Q�[��
	/// </summary>
	Game* m_game = nullptr;
	/// <summary>
	/// �v���C���[
	/// </summary>
	Player* m_player = nullptr;
	/////////////////////////////////////////�ϐ�
	/// <summary>
	/// �ő�HP
	/// </summary>
	int m_fullhp = 0;
	/// <summary>
	/// ����HP
	/// </summary>
	int m_nowhp = m_fullhp;
	/// <summary>
	/// �ߋ�HP
	/// </summary>
	int m_oldhp = m_fullhp;
	/// <summary>
	/// UIA�̈ʒu
	/// </summary>
	Vector2 m_positionA = { 0.0f,0.0f };
	/// <summary>
	/// UIB��C�̈ʒu
	/// </summary>
	Vector2 m_positionBC = { 0.0f,0.0f };
	/// <summary>
	/// ����hp�o�[�X�P�[��(�������̂�m_hpUI_B�Az�͌Œ�)
	/// </summary>
	Vector3 m_scale_B = { 1.0,1.0,1.0 };
	/// <summary>
	/// ���ݓ���hp�o�[�X�P�[��(�������̂�m_hpUI_C�Az�͌Œ�)
	/// </summary>
	Vector3 m_scale_C = { 1.0,1.0,1.0 };
	/// <summary>
	/// �����x���^�C�}�[
	/// </summary>
	float m_delaytimer = 0.0;
	/// <summary>
	/// ���ߌ�������
	/// </summary>
	Vector3 m_changeS_C = m_scale_C;
	/// <summary>
	/// ���ݓ��ߌ����l
	/// </summary>
	Vector3 m_nowScale = m_hpUI_C.GetScale();
	/// <summary>
	/// �z��̃i���o�[
	/// </summary>
	int m_Vectornum = 0;					
	/// <summary>
	/// �`���Ԃ������łȂ���
	/// </summary>
	bool m_isImage = false;
	/////////////////////////////////////////�����ݒ�n��
	/// <summary>
	/// HP��������
	/// </summary>
	/// <param name="decrease"></param>
	void DecreaseHP(int decrease)
	{
		//HP�������l���炷
		m_nowhp -= decrease;
		return;
	}
	/// <summary>
	/// HP�����ݒ�
	/// </summary>
	void SetEnemyHp()
	{
		m_fullhp = m_game->m_EnemyList[m_Vectornum]->m_hp;
		m_nowhp = m_fullhp;
		m_oldhp = m_fullhp;
	}
};

