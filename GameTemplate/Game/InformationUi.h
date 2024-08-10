#pragma once
#include "Game.h"


class Game;

class InformationUi : public IGameObject
{
public:
	/////////////////////////////////////////構造体
	struct InformationSpriteParam
	{
		/// <summary>
		/// ワイプ方向X軸
		/// </summary>
		Vector2 m_infoWipeDirX = {-1.0f,0.0f};
		/// <summary>
		/// ワイプ方向Y軸
		/// </summary>
		Vector2 m_infoWipeDirY = { 0.0f,-1.0f };
		/// <summary>
		/// ワイプサイズX
		/// </summary>
		float m_infoWipeSizeX = 0.0f;
		/// <summary>
		/// ワイプサイズY
		/// </summary>
		float m_infoWipeSizeY = 0.0f;
	};
	struct InformationSprite
	{
		/// <summary>
		/// インフォメーションスプライト
		/// </summary>
		SpriteRender m_InformationUi;
		/// <summary>
		/// ワイプサイズパラメータ
		/// </summary>
		InformationSpriteParam m_informationWipeParam;
		/// <summary>
		/// パラメーター登録
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
	/// 新しいスプライトを外部から登録する関数
	/// </summary>
	void InitInformationSprite(const char* listName, const char* filePath,const float w,const float h);
	/// <summary>
	/// アップデート関数
	/// </summary>
	void Update();
	/// <summary>
	/// ワイプ計算関数
	/// </summary>
	void WipeCalc();
	/// <summary>
	/// 描画関数
	/// </summary>
	void Render(RenderContext& rc);
	/////////////////////////////////////////変数
	/// <summary>
	/// 描画するかどうか
	/// </summary>
	bool m_isInfoDraw = false;
	/// <summary>
	/// ワイプの開閉
	/// </summary>
	bool m_isInfoWipe = true;
	/// <summary>
	/// ワイプが開ききったかどうか
	/// </summary>
	bool m_isWipeFullyOpen = false;
	/////////////////////////////////////////配列
	/// <summary>
	/// インフォメーションリスト
	/// </summary>
	std::map<std::string, InformationSprite*> m_informationList;
	/// <summary>
	/// インフォメーションリストイテレーター
	/// </summary>
	std::map<std::string, InformationSprite*>::iterator m_informationListIterator;
	/////////////////////////////////////////ポインタ変数
	/// <summary>
	/// ゲームのインスタンス取得用変数
	/// </summary>
	Game* m_game = nullptr;
	/////////////////////////////////////////メンバ関数
	/// <summary>
	/// リストにインフォメーションを登録
	/// </summary>
	void InitInformationList(const char* Name, InformationSprite* data)
	{
		std::string listName = Name;

		m_informationList.insert({ listName,data });
	}
	/// <summary>
	/// インフォメーションを実行
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
	/// 現在のワイプサイズXを取得
	/// </summary>
	/// <returns></returns>
	float GetNowIteratorWipeSizeX()
	{
		return m_informationListIterator->second->m_informationWipeParam.m_infoWipeSizeX;
	}
	/// <summary>
	/// 開いてるインフォメーションを閉じる関数
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
	/// 現在インフォメーション中かどうか
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

