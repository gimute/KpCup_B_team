#pragma once

//エネミーアタックポイントの数
#define ENEMY_ATTACK_POINT_NUM 20

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
		int m_number;				//アタックポイントの要素番号
		bool m_use = false;			//使用中か
	};

	Game();
	~Game();

	
	void Update();
	void Render(RenderContext& rc);
	void Delete_EnemyVec(const int num);

	/// <summary>
	/// エネミーの配列から配列番号を入れてその位置を取得
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	Vector3 GetEnemyListPos(int num);

	//エネミーアタックポイントの座標更新
	void EnemyAttackPointUpdate();

	//引数に渡された座標に一番近い使用可能なアタックポイントのアドレスを返す
	//アタックポインタが一定数以上使用中ならnullptrを返す
	EnemyAttackPoint* GetNearEnemyAttackPoint(Vector3 pos);

	//指定された要素番号のアタックポイントを使用中にする
	void EnemyAttackPointSetUse(int number)
	{
		if (m_enemyAttackPointList[number].m_use != true)
		{
			m_enemyAttackPointList[number].m_use = true;
		}	
	}
	//指定された要素番号のアタックポイントを未使用にする
	void EnemyAttackPointSetUnUse(int number)
	{
		if (m_enemyAttackPointList[number].m_use != false)
		{
			m_enemyAttackPointList[number].m_use = false;
		}
	}

	////引数に渡された座標に一番近い使用可能なアタックポイントのアドレスを返す
	////使用可能なアタックポイントが無ければnullptrを返す
	//EnemyAttackPoint* GetEnemyAttackPoint(Vector3 pos);
	////使用中アタックポイントの数のカウントを1減らすだけの関数
	//void useAttackPointNumDecrement()
	//{
	//	m_useAttackPointNum--;
	//}


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

