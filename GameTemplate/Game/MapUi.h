#pragma once

#include "Game.h"
#include "Enemy.h"
#include "Player.h"

class Game;
class Enemy;
class Player;

class MapUi : public IGameObject
{
public:
	/////////////////////////////////////////構造体
	/// <summary>
	/// 作成モード選択
	/// </summary>
	enum CreateMapEnemyPoint_Mode
	{
		m_Initialization_Mode,
		m_Add_Mode,
	};
	/// <summary>
	/// エネミーのポインタUi
	/// </summary>
	struct MapUi_EnemyPoint
	{
		/// <summary>
		/// エネミーポインタスプライト
		/// </summary>
		SpriteRender m_mapUiEnemyPoint;
		/// <summary>
		/// エネミークラスのインスタンス格納変数
		/// </summary>
		Enemy* m_enemy = nullptr;
		/// <summary>
		/// このポインタUiは描画するかどうか
		/// </summary>
		bool m_isDraw = false;
		/// <summary>
		/// コンストラクタ
		/// </summary>
		MapUi_EnemyPoint()
		{
			m_mapUiEnemyPoint.Init("Assets/modelData/ui_map/MapEnemyPoint.DDS", 30.0f, 30.0f);
		}
		/// <summary>
		/// エネミーのインスタンスを登録
		/// </summary>
		void SetEnemyInstance(Enemy& enemyinstance)
		{
			m_enemy = &enemyinstance;
		}
		/// <summary>
		/// 描画判定登録
		/// </summary>
		/// <param name="setBool"></param>
		void SetPointDrawBool(bool setBool)
		{
			m_isDraw = setBool;
		}
	};
	/////////////////////////////////////////関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MapUi();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~MapUi();
	/// <summary>
	/// スタート関数
	/// </summary>
	bool Start();
	/// <summary>
	/// アップデート関数
	/// </summary>
	void Update();
	/// <summary>
	/// マッププレイヤーポインタの更新
	/// </summary>
	void MapPlayerPointUpdate();
	/// <summary>
	/// マップエネミーポインタの更新
	/// </summary>
	void MapEnemyPointUpdate();
	/// <summary>
	/// マップエネミーポインタを作成する
	/// </summary>
	void CreateMapEnemyPoint(const CreateMapEnemyPoint_Mode mode);
	/// <summary>
	/// エネミーのポインタの描画判定処理
	/// </summary>
	void EnemyPointDisplay();
	/// <summary>
	/// マップ処理
	/// </summary>
	/// <param name="worldCenterPosition"></param>
	/// <param name="worldPosition"></param>
	/// <param name="mapPosition"></param>
	/// <returns></returns>
	void WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition
		, Vector3& mapPosition);
	/// <summary>
	/// マップの境界線
	/// </summary>
	/// <param name="targetpos"></param>
	bool IsMapLimited(const Vector3 determineTargetPos);
	/// <summary>
	/// 描画処理関数
	/// </summary>
	void Render(RenderContext& rc);
	/////////////////////////////////////////ポインタ変数
	/// <summary>
	/// Gameクラスのインスタンス格納変数
	/// </summary>
	Game* m_game = nullptr;
	/// <summary>
	/// Playerクラスのインスタンス格納変数
	/// </summary>
	Player* m_player = nullptr;
	/////////////////////////////////////////変数
	/// <summary>
	/// 現在のエネミーの数
	/// </summary>
	int nowenemynumber = -1;
	/////////////////////////////////////////スプライト
	/// <summary>
	/// マップフレームスプライト
	/// </summary>
	SpriteRender m_mapUiFrame;
	/// <summary>
	/// マップベーススプライト
	/// </summary>
	SpriteRender m_mapUiBase;
	/// <summary>
	/// マッププレイヤーポインタスプライト
	/// </summary>
	SpriteRender m_mapUiPlayerPoint;
	/////////////////////////////////////////配列
	/// <summary>
	/// エネミーポインタスプライトの構造体のアドレスを格納するmap
	/// </summary>
	std::map<int, MapUi_EnemyPoint*> m_mapUiEnemyPointList;
	/////////////////////////////////////////メンバ関数
};

