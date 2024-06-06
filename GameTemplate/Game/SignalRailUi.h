#pragma once

#include "SignalUi.h"

class SignalUi;
class Game;
class SignalRailUi : public IGameObject
{
public:
	/////////////////////////////////////////関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SignalRailUi();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SignalRailUi();
	/// <summary>
	/// スタート関数
	/// </summary>
	/// <returns></returns>
	bool Start();
	/// <summary>
	/// アップデート
	/// </summary>
	void Update();
	/// <summary>
	/// 攻撃ステートEnemyの探索
	/// </summary>
	void AttackEnemySearch();
	/// <summary>
	/// 危険信号Uiリスト削除
	/// </summary>
	/// <param name="num"></param>
	void DeleteSignalList(const int num);
	/// <summary>
	/// 正面にEnemyが存在するか判定する関数
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	bool AngleCheck(const Vector3& position);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="Listnum"></param>
	/// <returns></returns>
	bool ListCheck(int Listnum);
	/// <summary>
	/// 現在探索中のEnemyのステートがAttackステートかどうか調べる
	/// </summary>
	/// <param name="ListNum"></param>
	/// <returns></returns>
	bool EnemyStateIsAttack(int ListNum);
	/// <summary>
	/// モデルレンダー
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/////////////////////////////////////////メンバ変数
	/// <summary>
	/// シグナルレティクルスプライトレンダー
	/// </summary>
	SpriteRender m_ReticleUi;
	/// <summary>
	/// ゲーム
	/// </summary>
	Game* m_game = nullptr;
	/////////////////////////////////////////変数
	/// <summary>
	/// 危険信号Uiの数
	/// </summary>
	//int m_signalQua = 0;
	/// <summary>
	/// 危険信号Uiのリスト
	/// </summary>
	std::array<SignalUi*, 10> m_signalUiList_t;


	//使用中のシグナルUiの数
	int m_useSignalUiNum = 0;

	//std::vector<SignalUi*> m_signalUiList;
};

