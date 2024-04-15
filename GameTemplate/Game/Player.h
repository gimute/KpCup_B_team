#pragma once
class Game;
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
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Rotation();
	void Move();
	//void PlayAnimation();


//メンバ変数
	ModelRender m_modelRender;
	Vector3 m_position;				//座標
	CharacterController m_charaCon;	//キャラクターコントローラー
	Vector3 m_moveSpeed;			//移動速度。
	Quaternion m_rotation;	//クォータニオン。
	Player* m_player = nullptr;
	AnimationClip m_animationclips[enAnimationClip_Num];	//アニメーションクリップ

	Vector3 m_moveforward = Vector3::AxisZ;		//プレイヤーの移動用正面ベクトル。
	int m_speed = 0;

};

