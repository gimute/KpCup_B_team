#pragma once

#include "SignalUi.h"

class SignalUi;
class Game;
class SignalRailUi : public IGameObject
{
public:
	/////////////////////////////////////////�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SignalRailUi();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SignalRailUi();
	/// <summary>
	/// �X�^�[�g�֐�
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// �A�b�v�f�[�g
	/// </summary>
	void Update();
	/// <summary>
	/// �U���X�e�[�gEnemy�̒T��
	/// </summary>
	void AttackEnemySearch();
	/// <summary>
	/// �댯�M��Ui���X�g�폜
	/// </summary>
	/// <param name="num"></param>
	void DeleteSignalList(const int num);
	/// <summary>
	/// ���ʂ�Enemy�����݂��邩���肷��֐�
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	bool AngleCheck(const Vector3& position);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="Listnum"></param>
	/// <returns></returns>
	bool ListCheck(int Listnum);
	/// <summary>
	/// ���ݒT������Enemy�̃X�e�[�g��Attack�X�e�[�g���ǂ������ׂ�
	/// </summary>
	/// <param name="ListNum"></param>
	/// <returns></returns>
	bool EnemyStateIsAttack(int ListNum);
	/// <summary>
	/// ���f�������_�[
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/////////////////////////////////////////�����o�ϐ�
	/// <summary>
	/// �V�O�i�����e�B�N���X�v���C�g�����_�[
	/// </summary>
	SpriteRender m_ReticleUi;
	/// <summary>
	/// �Q�[��
	/// </summary>
	Game* m_game = nullptr;
	/////////////////////////////////////////�ϐ�
	/// <summary>
	/// �댯�M��Ui�̐�
	/// </summary>
	//int m_signalQua = 0;
	/// <summary>
	/// �댯�M��Ui�̃��X�g
	/// </summary>
	std::array<SignalUi*, 10> m_signalUiList_t;


	//�g�p���̃V�O�i��Ui�̐�
	int m_useSignalUiNum = 0;

	//std::vector<SignalUi*> m_signalUiList;
};

