#pragma once

#define LIST_MAX 3

class GameCamera;
class Enemy;
class Game;
class Player;
class EnemyCamPos
{
public:
	/// <summary>
	/// カメラ正面にいるエネミーを格納するための構造体
	/// </summary>
	struct EnemyCamForward 
	{
		/// <summary>
		/// リスト番号
		/// </summary>
		int m_num = -1;
		/// <summary>
		/// 登録中のEnemyListの番号
		/// </summary>
		int m_registNum = -1;
		/// <summary>
		/// リストに設定中であるかどうか
		/// </summary>
		bool m_enemyRegist = false;
	};
	/////////////////////////////////////////関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyCamPos();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyCamPos();
	/// <summary>
	/// カメラとエネミーの位置を確認し
	/// 位置と距離によってEnemyの攻撃頻度を
	/// 変更する関数
	/// </summary>
	float EnemyCamPosConfirmation(Enemy* enemy);
	/// <summary>
	/// 正面にEnemyが存在するか判定する関数
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	bool AngleCheck(const Vector3& position);
	/// <summary>
	/// m_camForwardListに空の要素があったら登録し
	/// 登録したEnemyの攻撃頻度を"高"する関数
	/// </summary>
	/// <param name="ListNum"></param>
	void Registration(int ListNum);
	/// <summary>
	/// m_camForwardListに空の要素がない場合にそのEnemyと
	/// 現在登録中のEnemyの中で一番カメラから遠いEnemyを比べ
	/// 近い方をm_camForwardLisに登録し攻撃頻度を"高"する関数
	/// </summary>
	/// <param name="ListNum"></param>
	/// <param name="enemypos"></param>
	void CompareNear(int ListNum,const Vector3& enemypos);
	/// <summary>
	/// カメラ範囲内にいないEnemyの攻撃頻度を"低"にする
	/// またはカメラ範囲内におらず、m_camForwardListに登録中の
	/// 要素を未登録状態にしEnemyの攻撃頻度を"低"にする関数
	/// </summary>
	/// <param name="ListNum"></param>
	void CamOut(int ListNum,const Vector3 enemypos);
	/// <summary>
	///	m_camForwardListに空きがあるかどうかを調べる関数
	/// </summary>
	bool ListCheck();
	/// <summary>
	/// m_camForwardListに同じ要素番号があるかどうかを調べる関数
	/// </summary>
	/// <param name="ListNum"></param>
	/// <returns></returns>
	bool ListCheck(int ListNum);
	/// <summary>
	/// 現在探索中のEnemyのステートがAttackステートかどうか調べる
	/// </summary>
	/// <param name="ListNum"></param>
	/// <returns></returns>
	bool EnemyStateIsAttack(int ListNum);
	/////////////////////////////////////////メンバ変数
	/// <summary>
	/// ゲーム
	/// </summary>
	Game* m_game = nullptr;
	/// <summary>
	/// ゲームカメラ
	/// </summary>
	GameCamera* m_camera = nullptr;
	/// <summary>
	/// プレイヤー
	/// </summary>
	Player* m_player = nullptr;
	/////////////////////////////////////////変数
	/// <summary>
	/// カメラ正面エネミーの構造体配列
	/// </summary>
	EnemyCamForward m_camForwardList[LIST_MAX];
	/// <summary>
	///	現在の空き要素
	/// </summary>
	int m_emptyPoint = 0;
	/////////////////////////////////////////初期設定系統
	bool PointerNull()
	{
		if (m_game == nullptr||m_player == nullptr||m_camera == nullptr)
		{
			return true;
		}

		return false;
	}
};

