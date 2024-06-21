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
		/// コンストラクタ
		/// </summary>
		MapUi_EnemyPoint()
		{
			m_mapUiEnemyPoint.Init("Assets/modelData/ui_map/MapEnemyPoint.DDS", 30.0f, 30.0f);
		}
		/// <summary>
		/// エネミーのインスタンスを登録
		/// </summary>
		void SetEnemyInstance(Enemy* enemyinstance)
		{
			m_enemy = enemyinstance;
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
	/// マップエネミーポインタを作成する
	/// </summary>
	void CreateMapEnemyPoint();
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

