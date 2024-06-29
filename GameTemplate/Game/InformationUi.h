#pragma once
class InformationUi : public IGameObject
{
public:
	/////////////////////////////////////////構造体
	struct InformationSpriteParam
	{
		/// <summary>
		/// インフォメーションスプライト
		/// </summary>
		SpriteRender m_InformationUi;
		/// <summary>
		/// ワイプサイズ
		/// </summary>
		float m_informationwipeSize = 0.0f;
		/// <summary>
		/// パラメーター登録
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
	/////////////////////////////////////////関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	InformationUi();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~InformationUi();
	/// <summary>
	/// スタート関数
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// アップデート関数
	/// </summary>
	void Update();
	/// <summary>
	/// 描画関数
	/// </summary>
	void Render(RenderContext& rc);
	/////////////////////////////////////////変数
	/// <summary>
	/// 描画するかどうか
	/// </summary>
	bool m_isInfoDraw = false;
	/////////////////////////////////////////配列
	/// <summary>
	/// インフォメーションリスト
	/// </summary>
	std::map<std::string, InformationSpriteParam*> m_informationList;
	/// <summary>
	/// インフォメーションリストイテレーター
	/// </summary>
	std::map<std::string, InformationSpriteParam*>::iterator m_informationListIterator;
	/////////////////////////////////////////メンバ関数
	/// <summary>
	/// リストにインフォメーションを登録
	/// </summary>
	void InitInformationList(const char* Name, InformationSpriteParam* data)
	{
		std::string listName = "info1";

		m_informationList.insert({ listName,data });
	}
	/// <summary>
	/// インフォメーションを実行
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

