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
		enAnimationClip_knockdown,
		enAnimationClip_Num				//アニメーション数
	};
	//プレイヤーステート
	enum EnPlayerState {
		enPlayerState_Idle,				//待機。
		enPlayerState_Walk,				//歩き。
		enPlayerState_PostureWalk,		//構え歩き。
		enPlayerState_Attack,			//攻撃
		enPlayerState_Rolling,
		enPlayerState_ReceiveDamage,		//被ダメージ
		enPlayerState_KnockDown,
		enPlayerState_Event				//イベントシーン中
	};

	enum EnEvent {
		enGameClear,
		enNone
	};

	struct EventInfo
	{
		EnEvent m_eventState = enNone;						//イベントの種類
		Vector3 m_targetPos;								//目標地点
		EnAnimationClip m_motion = enAnimationClip_Idle;	//再生するモーション
		float m_waitTime = 0.0f;							//目的地に着いた後の待機時間
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
	void RollingEndRot();
	bool AngleCheck(const Vector3 forward,  const Vector3& position);
	void Move();
	void Rolling();						//回避処理
	void ManageState();					//ステート遷移処理
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

	//イベント設定
	void SetEvent(EnEvent eventnum);

//メンバ変数
	ModelRender m_modelRender;
	Vector3 m_position;										//座標
	CharacterController m_charaCon;							//キャラクターコントローラー
	CollisionObject* m_collisionObject;						//コリジョンオブジェクト
	CollisionObject* m_justRollingCollisionObject = nullptr;						//ジャスト回避用コリジョン
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
	SoundSource* m_gunStance = nullptr;
	int m_speed = 0;
	Game* m_game = nullptr;									//ゲーム
	SphereCollider m_sphereCollider;						//コリダー
	bool shot= false;
	Vector3 m_rollingVec = Vector3::Zero;					//回避方向
	Vector3 m_rollingSpeed = Vector3::Zero;					//回避スピード
	float rollingCoolDownTime = 0.5;						//回避クールダウン時間
	float m_muteki_timer = 0.0f;							//無敵タイマー
	float m_reset_timer = 0.0f;								//リセットタイマー
	float m_justRollingTime = 0.0f;
	bool m_justRollingCol = false;
	float m_rollingCorrectionTime = 0.0f;

private:
	EventInfo m_eventInfos[3] = {
		{enGameClear,{150.0f,0.0f,1230.0f},enAnimationClip_Idle,1.0},
		{enGameClear,{1200.0f,0.0f,1230.0f},enAnimationClip_Walk,0.0f},
		{enNone,{0.0f,0.0f,0.0f},enAnimationClip_Idle,0.0f}
	};
	int m_eventInfoNum = 0;
	float m_eventTimer = 0.0f;

	float m_gameoverWaitTimer = 0.0f;
	bool m_mutekiModelDraw = false;
};

