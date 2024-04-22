#pragma once
class Game;
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
		enAnimationClip_Num				//アニメーション数
	};
	//プレイヤーステート
	enum EnPlayerState {
		enPlayerState_Idle,				//待機。
		enPlayerState_Walk,				//歩き。
		enPlayerState_Attack,			//攻撃
	};
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void PlayAnimation();
	void Rotation();
	void Move();
	void ManageState();					//ステート遷移処理
	//void PlayAnimation();
	void ProcessCommonStateTransition();	//共通処理
	void ProcessIdleStateTransition();		//待機遷移
	void ProcessWalkStateTransition();		//歩き遷移


//メンバ変数
	ModelRender m_modelRender;
	Vector3 m_position;				//座標
	CharacterController m_charaCon;	//キャラクターコントローラー
	Vector3 m_moveSpeed;			//移動速度。
	Quaternion m_rotation;	//クォータニオン。
	Player* m_player = nullptr;
	AnimationClip m_animationclips[enAnimationClip_Num];	//アニメーションクリップ
	EnPlayerState m_playerstate = enPlayerState_Idle;
	Bullet* m_bullet = nullptr;
	Vector3 m_forward = Vector3::AxisZ;		//プレイヤーの移動用正面ベクトル。
	int m_speed = 0;

};

