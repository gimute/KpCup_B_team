#pragma once
class Game;
class Player;
class Enemy : public IGameObject
{
public:
	//行動ステート
	enum EnEnemyState {
		enEnemyState_Idle,			//待機
		enEnemyState_Chase,			//追跡
		enEnemyState_Attack,		//攻撃
		//enEnemyState_ReceiveDamage,	//被ダメージ
		//enEnemyState_Down,			//ダウン
	};
	//アニメーション類/////////////////////////////
//アニメーションステート
	enum EnAnimationClip {
		enAnimationClip_Idle,			//棒立ち
		enAnimationClip_Chase,			//追跡
		enAnimationClip_Attack,			//攻撃
		//enAnimationClip_ReceiveDamage,	//被ダメージ
		//enAnimationClip_Down,			//ダウン
		enAnimationClip_Num,			//アニメーション数
	};
	void PlayAnimation();
	//ステート遷移関数///////////////////////////////////////////
	void ManageState();							//遷移処理
	void ProcessCommonStateTransition();		//共通遷移処理
	void ProcessIdleStateTransition();			//待機遷移
	void ProcessChaseStateTransition();			//追跡遷移
	void ProcessAttackStateTransition();		//攻撃遷移
	void ProcessReceiveDamageStateTransition();	//被ダメ遷移
	void ProcessDownStateTransition();			//ダウン遷移
	/////////////////////////////////////////////////////////////
	Enemy() {};
	~Enemy();
	bool Start();						//アップデート
	void Update();                                         //アップデート
	void Rotation();                                       //回転
	void Chase();										   //追跡
	void Attack();										   //攻撃
	//void Collision();										//本体の当たり判定
	const bool SearchPlayer() const;                       //プレイヤー探知
	const bool SearchChaseDistance() const;					//追跡距離探知
	const bool SearchAttackDistance() const;               //攻撃距離探知
	void Render(RenderContext& rc);                        //モデルレンダー
	//bool IsAttack() const				//移動できるかどうか
	//{
	//	return m_enemystate != enAnimationClip_Chase &&
	//		m_enemystate != enAnimationClip_Attack;
	//}
	///////////////////////////////////////////////////////////
	//初期設定系統
	//メンバ関数宣言
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	Game* m_game = nullptr;
	//CollisionObject* m_attackcoll;		//コリジョンオブジェクト。
	AnimationClip m_animationclips[enAnimationClip_Num];     //アニメーションクリップ
	EnEnemyState m_enemystate = enEnemyState_Idle;          //エネミーステート
	Vector3	m_forward = Vector3::AxisZ;						//エネミーの正面ベクトル。
	Vector3 m_movespeed;									//移動速度
	Vector3 m_position;										//座標
	Quaternion m_rotation;					//回転
	Vector3 m_scale = Vector3::One;			//大きさ
	ModelRender m_modelRender;	//モデルレンダー
	//CollisionObject* m_collisionObject;		//コリジョンオブジェクト。
	CharacterController m_charaCon;        //キャラコン
	int m_hp = 0;                           //HP
	int m_sh = 0;							//シールド
	int m_Vectornum = 0;					//配列のナンバー
	float m_idleTimer= 0.0f;								//待機時間タイマー。
	float m_chaseTimer = 0.0f;								//追跡時間タイマー。
	float m_attackTimer = 5.0f;								//攻撃時間タイマー。
	float attacktime = 5.0f;
	float m_attackGotimer = 0.0;							//攻撃時間タイマー
	float attackGotime = 2.0;								//攻撃時間
	float m_attackcooltimer = 0.0f;
	float attackcooltime = 3.0f;
	float m_looptimer = 0.0f;								//ループタイマー
	float looptime = 1.16;									//ループ時間
	float m_mutekitimer = 0.0f;								//無敵タイマー
	float mutekitime = 0.1f;								//無敵時間
};

