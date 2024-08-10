#pragma once
#include "Game.h"
#include "InformationUi.h"

class Game;
class InformationUi;

class CountDownUI : public IGameObject
{
public:
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CountDownUI();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~CountDownUI();
	/// <summary>
	/// �X�^�[�g�֐�
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// �A�b�v�f�[�g�֐�
	/// </summary>
	void Update();
	/// <summary>
	/// �A���t�@����
	/// </summary>
	void AlphaCalc();
	/// <summary>
	/// �`��֐�
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);

	bool GetFirstInfo()
	{
		return m_isFirstInfo;
	}

	void SetFirstInfo(bool FirstInfo)
	{
		m_isFirstInfo = FirstInfo;
		return;
	}

	bool GetSecondInfo()
	{
		return m_isSecondInfo;
	}

	void SetSecondInfo(bool SecondInfo)
	{
		m_isSecondInfo = SecondInfo;
	}
	
	bool GetThirdCoDown()
	{
		return m_isThirdCoDown;
	}

	void SetThirdCoDown(bool ThirdCoDown)
	{
		m_isThirdCoDown = ThirdCoDown;
	}

	bool GetSecondCoDown()
	{
		return m_isSecondCoDown;
	}

	void SetSecondCoDown(bool SecondCoDown)
	{
		m_isSecondCoDown = SecondCoDown;
	}

	bool GetFirstCoDown()
	{
		return m_isFirstCoDown;
	}

	void SetFirstCoDown(bool FirstCoDown)
	{
		m_isFirstCoDown = FirstCoDown;
	}

	bool GetStartCoDown()
	{
		return m_isStartCoDown;
	}

	void SetStartCoDown(bool StartCoDown)
	{
		m_isStartCoDown = StartCoDown;
	}

	bool GetCountDownClose()
	{
		return m_isCountDownClose;
	}

	void SetCountDownClose(bool CountDownClose)
	{
		m_isCountDownClose = CountDownClose;
	}


private:
	enum m_Draw
	{
		ThirdDraw,
		SecondDraw,
		FirsrDraw,
		StartDraw
	};
	SpriteRender	 m_spriteRenderThird;			// �u�R�v�̃X�v���C�g�����_�[
	SpriteRender	 m_spriteRenderSecond;			// �u�Q�v�̃X�v���C�g�����_�[
	SpriteRender	 m_spriteRenderFirst;			// �u�P�v�̃X�v���C�g�����_�[
	SpriteRender	 m_spriteRenderStart;			// �uStart�v�̃X�v���C�g�����_�[
	Game*			 m_game = nullptr;				// �Q�[���̃C���X�^���X�擾�p�ϐ�
	GameTimer*		 m_gameTimer = nullptr;			// �Q�[���^�C�}�[�̃C���X�^���X�擾�p�ϐ�
	InformationUi*	 m_infoUi = nullptr;			// �C���t�H�̃C���X�^���X�擾�p�ϐ�
	bool			 m_isCountDraw = false;			// �`�悷�邩�ǂ���
	bool			 m_isCoDoWipe = false;			// ���C�v�̊J��
	bool			 m_isWipeFullOpen = false;		// ���C�v���J�����������ǂ���
	bool			 m_isSecondDraw = false;		// �Q�̉摜��\�����邩�ǂ���
	bool			 m_isFirstDraw = false;			// �P�̉摜��\�����邩�ǂ���
	bool			 m_isStartDraw = false;			// �X�^�[�g�̉摜��\�����邩�ǂ���
	//bool			 m_isCountDownClose = false;	// ���ׂẴJ�E���g�_�E���̕`�悪�������ǂ���
	bool			 m_isFirstInfo = false;			// �P�Ԗ�(�~�b�V����)�̉摜���\������Ă��邩
	bool			 m_isSecondInfo = false;		// �Q�Ԗ�(�������)�̉摜���\������Ă��邩
	//m_Draw			 m_DrawState = ThirdDraw;

	bool			 m_isThirdCoDown = false;		// �R�̉摜���\������Ă��邩
	bool			 m_isSecondCoDown = false;		// �Q�̉摜���\������Ă��邩
	bool			 m_isFirstCoDown = false;		// �P�̉摜���\������Ă��邩
	bool			 m_isStartCoDown = false;		// �X�^�[�g�̉摜���\������Ă��邩
	bool			 m_thirdCD = false;
	bool			 m_secondCD = false;
	bool			 m_firstCD = false;
	bool			 m_startCD = false;
	float			 m_minit = 0.0f;				// �b���̕ϐ�
public:
	bool			 m_isCountDownClose = false;	// ���ׂẴJ�E���g�_�E���̕`�悪�������ǂ���
	//m_Draw GetDrawState()
	//{
	//	return m_DrawState;
	//}
	//void SetDrawState(m_Draw State)
	//{
	//	m_DrawState = State;
	//}
};

