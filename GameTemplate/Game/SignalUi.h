#pragma once
class GameCamera;
class SignalRailUi;
class Game;
class Player;
class SignalUi : public IGameObject
{
public:
	/////////////////////////////////////////関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SignalUi();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SignalUi();
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
	/// 回転
	/// </summary>
	void Rotation();
	/// <summary>
	/// 危険信号Ui削除
	/// </summary>
	void DeleteSignal();
	/// <summary>
	/// モデルレンダー
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);
	/////////////////////////////////////////メンバ変数
	/// <summary>
	/// シグナルスプライトレンダー
	/// </summary>
	SpriteRender m_SignalUi;
	/// <summary>
	/// ゲームカメラ
	/// </summary>
	GameCamera* m_gameCamera = nullptr;
	/// <summary>
	/// シグナルレール
	/// </summary>
	SignalRailUi* m_signalRailUi = nullptr;
	/// <summary>
	/// ゲーム
	/// </summary>
	Game* m_game = nullptr;
	/// <summary>
	/// プレイヤー
	/// </summary>
	Player* m_player = nullptr;
	/////////////////////////////////////////変数
	/// <summary>
	/// の配列番号を格納する変数
	/// </summary>
	int m_enemyConnectNum = -1;
	/// <summary>
	///	配列番号
	/// </summary>
	int m_VecNum = -1;
	/// <summary>
	/// 2D空間用の上方向のベクトルzは0.0固定
	/// </summary>
	Vector3 m_up2D = { 0.0f,1.0f,0.0f };
	/// <summary>
	/// 2D空間用の差分格納用ベクトル
	/// </summary>
	Vector3 m_diff2D = Vector3::Zero;
	/// <summary>
	/// 削除タイマー
	/// </summary>
	float m_deleteTimer = 0.0f;
	/////////////////////////////////////////初期設定系統
	void SetEnemyVecNum(int num)
	{
		m_enemyConnectNum = num;
		return;
	}
};

