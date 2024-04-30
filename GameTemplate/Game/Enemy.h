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
	~Enemy() {};
	bool Start();						//アップデート
	void Update();                                         //アップデート
	//モデルの回転をする関数
	void Rotation();                                       //回転
	void Chase();										   //追跡
	void Attack();										   //攻撃
	//void Collision();										//本体の当たり判定
	
	//プレイヤーが視界内に居るか確かめる関数
	const bool SearchPlayer() const;

	//プレイヤーが一定範囲内に居るか確かめる関数
	const bool SearchChaseDistance() const;

	//プレイヤーが攻撃範囲内に居るか確かめる関数
	const bool SearchAttackDistance() const;

	void Render(RenderContext& rc);
	///////////////////////////////////////////////////////////
	//初期設定系統
	//メンバ関数宣言
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	Game* m_game = nullptr;
	AnimationClip m_animationclips[enAnimationClip_Num];     //アニメーションクリップ
	EnEnemyState m_enemystate = enEnemyState_Idle;          //エネミーステート
	Vector3	m_forward = Vector3::AxisZ;						//エネミーの正面ベクトル。
	Vector3 m_movespeed;									//移動速度
	Vector3 m_position;										//座標
	Quaternion m_rotation;					//回転
	Vector3 m_scale = Vector3::One;			//大きさ
	ModelRender m_modelRender;	//モデルレンダー
	CharacterController m_charaCon;        //キャラコン
};

