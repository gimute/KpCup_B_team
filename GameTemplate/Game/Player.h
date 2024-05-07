#pragma once
class Game;
class Enemy;
class Bullet;
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
		enAnimationClip_Num				//アニメーション数
	};
	//プレイヤーステート
	enum EnPlayerState {
		enPlayerState_Idle,				//待機。
		enPlayerState_Walk,				//歩き。
		enPlayerState_PostureWalk,		//構え歩き。
		enPlayerState_Attack,			//攻撃
		enPlayerState_Rolling,
	};

	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void PlayAnimation();
	void Rotation();
	void AttackRotation();
	bool AngleCheck(const Vector3& position);
	void Move();
	void Rolling();						//回避処理
	void ManageState();					//ステート遷移処理
	//アニメーションイベント
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//void PlayAnimation();
	void ProcessCommonStateTransition();	//共通処理
	void ProcessIdleStateTransition();		//待機遷移
	void ProcessWalkStateTransition();		//歩き遷移
	void ProcessAttackStateTransition();    //攻撃遷移
	void ProcessRollingStateTransition();	//回避遷移
	//座標取得
	const Vector3& GetPosition() const
	{
		return m_position;
	}

//メンバ変数
	ModelRender m_modelRender;
	Vector3 m_position;										//座標
	CharacterController m_charaCon;							//キャラクターコントローラー
	Vector3 m_moveSpeed;									//移動速度。
	Quaternion m_rotation;									//クォータニオン。
	Player* m_player = nullptr;
	AnimationClip m_animationclips[enAnimationClip_Num];	//アニメーションクリップ
	EnPlayerState m_playerstate = enPlayerState_Idle;
	Bullet* m_bullet = nullptr;
	Vector3 m_forward = Vector3::AxisZ;						//プレイヤーの移動用正面ベクトル。
	Vector3 m_up = Vector3::AxisY;
	int m_speed = 0;
	Game* m_game = nullptr;									//ゲーム
	SphereCollider m_sphereCollider;						//コリダー
	bool shot= false;
	Vector3 m_rollingVec = Vector3::Zero;					//回避方向
	Vector3 m_rollingSpeed = Vector3::Zero;					//回避スピード
	float m_rollingCoolDown = 0.0f;							//回避クールダウン
	float rollingCoolDownTime = 0.5;						//回避クールダウン時間
};

