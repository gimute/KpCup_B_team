#pragma once
class Game;
class Enemy;
class Bullet;
class Collision;
class Player : public IGameObject
{
public:
	//アニメーションクリップ
	enum EnAnimationClip {
		enAnimationClip_Walk,			//歩き
		enAnimationClip_Idle,			//棒立ち
		enAnimationClip_Crouching,      //しゃがみ
		enAnimationClip_Punch,			//パンチモーション
		enAnimationClip_Gunshot,		//射撃モーション
		enAnimationClip_PostureWalk,	//構え歩き
		enAnimationClip_PostureIdle,	//構え立ち
		enAnimationClip_Rolling,		//ローリング
		enAnimationClip_Damage,			//被ダメージモーション
		enAnimationClip_Num				//アニメーション数
	};
	//プレイヤーステート
	enum EnPlayerState {
		enPlayerState_Idle,				//待機。
		enPlayerState_Walk,				//歩き。
		enPlayerState_PostureWalk,		//構え歩き。
		enPlayerState_Attack,			//攻撃
		enPlayerState_Rolling,
		enPlayerState_ReceiveDamage		//被ダメージ
	};

	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void PlayAnimation();
	void Rotation();
	void Collision();
	void AttackRotation();
	bool AngleCheck(const Vector3& position);
	void Move();
	void Rolling();						//回避処理
	void ManageState();					//ステート遷移処理
	void TimeAdjustment();				//時間計変数加減処理
	//アニメーションイベント
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//void PlayAnimation();
	void ProcessCommonStateTransition();		//共通処理
	void ProcessIdleStateTransition();			//待機遷移
	void ProcessWalkStateTransition();			//歩き遷移
	void ProcessAttackStateTransition();		//攻撃遷移
	void ProcessRollingStateTransition();		//回避遷移
	void ProcessReceiveDamageStateTransition();	//被ダメージ遷移
	//座標取得
	const Vector3& GetPosition() const
	{
		return m_position;
	}

//メンバ変数
	ModelRender m_modelRender;
	Vector3 m_position;										//座標
	CharacterController m_charaCon;							//キャラクターコントローラー
	CollisionObject* m_collisionObject;						//コリジョンオブジェクト
	Vector3 m_moveSpeed;									//移動速度。
	Quaternion m_rotation;									//クォータニオン。
	Vector3 m_scale = Vector3::One;			//大きさ
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	AnimationClip m_animationclips[enAnimationClip_Num];	//アニメーションクリップ
	EnPlayerState m_playerstate = enPlayerState_Idle;
	Bullet* m_bullet = nullptr;
	Vector3 m_forward = Vector3::AxisZ;						//プレイヤーの移動用正面ベクトル。
	Vector3 m_up = Vector3::AxisY;
	//int m_hp = 1000;	//無敵時間がないのでとりあえず大きい値にしておく
	int m_speed = 0;
	Game* m_game = nullptr;									//ゲーム
	SphereCollider m_sphereCollider;						//コリダー
	bool shot= false;
	Vector3 m_rollingVec = Vector3::Zero;					//回避方向
	Vector3 m_rollingSpeed = Vector3::Zero;					//回避スピード
	float m_rollingCoolDown = 0.0f;							//回避クールダウン
	float rollingCoolDownTime = 0.5;						//回避クールダウン時間
	float m_muteki_timer = 0.0f;							//無敵タイマー
	float m_reset_timer = 0.0f;								//リセットタイマー

private:


};

