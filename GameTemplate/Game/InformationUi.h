#pragma once
#include "Game.h"


class Game;

class InformationUi : public IGameObject
{
public:
	/////////////////////////////////////////�\����
	struct InformationSpriteParam
	{
		/// <summary>
		/// ���C�v����X��
		/// </summary>
		Vector2 m_infoWipeDirX = {-1.0f,0.0f};
		/// <summary>
		/// ���C�v����Y��
		/// </summary>
		Vector2 m_infoWipeDirY = { 0.0f,-1.0f };
		/// <summary>
		/// ���C�v�T�C�YX
		/// </summary>
		float m_infoWipeSizeX = 0.0f;
		/// <summary>
		/// ���C�v�T�C�YY
		/// </summary>
		float m_infoWipeSizeY = 0.0f;
	};
	struct InformationSprite
	{
		/// <summary>
		/// �C���t�H���[�V�����X�v���C�g
		/// </summary>
		SpriteRender m_InformationUi;
		/// <summary>
		/// ���C�v�T�C�Y�p�����[�^
		/// </summary>
		InformationSpriteParam m_informationWipeParam;
		/// <summary>
		/// �p�����[�^�[�o�^
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="w"></param>
		/// <param name="h"></param>
		/// <param name="wipeSizeInitialization"></param>
		void InitInformationSpriteParam(const char* filePath, const float w, const float h)
		{
			SpriteInitData informationSpriteInitData;

			informationSpriteInitData.m_ddsFilePath[0] = filePath;

			informationSpriteInitData.m_fxFilePath = "Assets/shader/infoWipe.fx";

			informationSpriteInitData.m_expandConstantBuffer = &m_informationWipeParam;
			informationSpriteInitData.m_expandConstantBufferSize = sizeof(m_informationWipeParam);

			informationSpriteInitData.m_width = static_cast<UINT>(w);
			informationSpriteInitData.m_height = static_cast<UINT>(h);

			m_InformationUi.Init(informationSpriteInitData);

			m_InformationUi.SetPosition({ 0.0f,0.0f,0.0f });
		}
	};
	/////////////////////////////////////////�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	InformationUi();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~InformationUi();
	/// <summary>
	/// �X�^�[�g�֐�
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// �V�����X�v���C�g���O������o�^����֐�
	/// </summary>
	void InitInformationSprite(const char* listName, const char* filePath,const float w,const float h);
	/// <summary>
	/// �A�b�v�f�[�g�֐�
	/// </summary>
	void Update();
	/// <summary>
	/// ���C�v�v�Z�֐�
	/// </summary>
	void WipeCalc();
	/// <summary>
	/// �`��֐�
	/// </summary>
	void Render(RenderContext& rc);
	/////////////////////////////////////////�ϐ�
	/// <summary>
	/// �`�悷�邩�ǂ���
	/// </summary>
	bool m_isInfoDraw = false;
	/// <summary>
	/// ���C�v�̊J��
	/// </summary>
	bool m_isInfoWipe = true;
	/// <summary>
	/// ���C�v���J�����������ǂ���
	/// </summary>
	bool m_isWipeFullyOpen = false;
	/////////////////////////////////////////�z��
	/// <summary>
	/// �C���t�H���[�V�������X�g
	/// </summary>
	std::map<std::string, InformationSprite*> m_informationList;
	/// <summary>
	/// �C���t�H���[�V�������X�g�C�e���[�^�[
	/// </summary>
	std::map<std::string, InformationSprite*>::iterator m_informationListIterator;
	/////////////////////////////////////////�|�C���^�ϐ�
	/// <summary>
	/// �Q�[���̃C���X�^���X�擾�p�ϐ�
	/// </summary>
	Game* m_game = nullptr;
	/////////////////////////////////////////�����o�֐�
	/// <summary>
	/// ���X�g�ɃC���t�H���[�V������o�^
	/// </summary>
	void InitInformationList(const char* Name, InformationSprite* data)
	{
		std::string listName = Name;

		m_informationList.insert({ listName,data });
	}
	/// <summary>
	/// �C���t�H���[�V���������s
	/// </summary>
	void InitGOInformation(const char* Name)
	{
		if (m_isInfoDraw)
		{
			return;
		}

		m_informationListIterator = m_informationList.find(Name);

		if (m_informationListIterator != m_informationList.end())
		{
			m_isInfoDraw = true;
			//m_game->IsPlayerMove(true);
			//m_game->SlowStart(true, 0);
			
			//if (m_informationListIterator == m_informationList.end())
			//{
			//	m_countDownUI->m_isCountDraw = true;
			//}
		}

	}
	/// <summary>
	/// ���݂̃��C�v�T�C�YX���擾
	/// </summary>
	/// <returns></returns>
	float GetNowIteratorWipeSizeX()
	{
		return m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeX;
	}
	/// <summary>
	/// �J���Ă�C���t�H���[�V���������֐�
	/// </summary>
	void InformationClose(bool slowBool)
	{
		if (!m_isWipeFullyOpen)
		{
			return;
		}

		m_isInfoWipe = false;
		//m_game->IsPlayerMove(false);
		if (slowBool)
		{
			g_gameTime->IsSlowMotion(false);
		}
	}
	/// <summary>
	/// ���݃C���t�H���[�V���������ǂ���
	/// </summary>
	/// <returns></returns>
	bool IsInformationOpen()
	{
		if (m_isInfoDraw)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

