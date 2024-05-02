#pragma once

//エネミーアタックポイントの数
#define ENEMY_ATTACK_POINT_NUM 10

///////////////////////////////
class Player;
class BackGround;
class GameCamera;
class MiniTimer;
class SpeedMeter;
class MainTimer;
class HpUi;
class Enemy;
class EnemyHpUi;
//
class Game : public IGameObject
{
public:
	//エネミーの攻撃可能ポイント
	//このポイントを確保できているエネミーのみ攻撃可能にする
	struct EnemyAttackPoint
	{
		Vector3 m_position;			//アタックポイントの座標
		bool m_use = false;			//使用中か
	};

	Game();
	~Game();

	
	void Update();
	void Render(RenderContext& rc);
	void Delete_EnemyVec(const int num);

	//エネミーアタックポイントの座標更新
	void EnemyAttackPointUpdate();
	//使用可能なエネミーアタックポイントのアドレスの取得
	//使用可能なポイントが無ければnullptrを返す
	EnemyAttackPoint* GetEnemyAttackPoint();

//メンバ変数
	Player* m_player;
	BackGround* m_background;
	GameCamera* m_gamecamera;
	MiniTimer* m_minitimer = nullptr;
	MainTimer* m_maintimer = nullptr;
	SpeedMeter* m_speedmeter = nullptr;
	FontRender m_fontrender;	//フォントレンダー。
	HpUi* m_hpui = nullptr;
	std::vector<Enemy*> m_EnemyList;
	std::vector<EnemyHpUi*> m_EnemyHpUiList;
	int m_EnemyQua = 0;

	EnemyAttackPoint m_enemyAttackPointList[ENEMY_ATTACK_POINT_NUM];
};

