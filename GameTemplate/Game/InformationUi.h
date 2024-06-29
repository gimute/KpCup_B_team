#pragma once
class InformationUi : public IGameObject
{
public:
	/////////////////////////////////////////�\����
	struct InformationSpriteParam
	{
		/// <summary>
		/// �C���t�H���[�V�����X�v���C�g
		/// </summary>
		SpriteRender m_InformationUi;
		/// <summary>
		/// ���C�v�T�C�Y
		/// </summary>
		float m_informationwipeSize = 0.0f;
		/// <summary>
		/// �p�����[�^�[�o�^
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="w"></param>
		/// <param name="h"></param>
		/// <param name="wipeSizeInitialization"></param>
		void InitInformationSpriteParam(const char* filePath, const float w, const float h, const float wipeSizeInitialization)
		{
			SpriteInitData informationSpriteInitData;

			informationSpriteInitData.m_ddsFilePath[0] = filePath;

			informationSpriteInitData.m_fxFilePath = "";

			m_informationwipeSize = wipeSizeInitialization;

			informationSpriteInitData.m_expandConstantBuffer = &m_informationwipeSize;
			informationSpriteInitData.m_expandConstantBufferSize = sizeof(m_informationwipeSize);

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
	/// �A�b�v�f�[�g�֐�
	/// </summary>
	void Update();
	/// <summary>
	/// �`��֐�
	/// </summary>
	void Render(RenderContext& rc);
	/////////////////////////////////////////�ϐ�
	/// <summary>
	/// �`�悷�邩�ǂ���
	/// </summary>
	bool m_isInfoDraw = false;
	/////////////////////////////////////////�z��
	/// <summary>
	/// �C���t�H���[�V�������X�g
	/// </summary>
	std::map<std::string, InformationSpriteParam*> m_informationList;
	/// <summary>
	/// �C���t�H���[�V�������X�g�C�e���[�^�[
	/// </summary>
	std::map<std::string, InformationSpriteParam*>::iterator m_informationListIterator;
	/////////////////////////////////////////�����o�֐�
	/// <summary>
	/// ���X�g�ɃC���t�H���[�V������o�^
	/// </summary>
	void InitInformationList(const char* Name, InformationSpriteParam* data)
	{
		std::string listName = "info1";

		m_informationList.insert({ listName,data });
	}
	/// <summary>
	/// �C���t�H���[�V���������s
	/// </summary>
	void InitGOInformation(const char* Name)
	{
		m_informationListIterator = m_informationList.find(Name);

		if (m_informationListIterator != m_informationList.end())
		{
			m_isInfoDraw = true;
		}
	}
};

