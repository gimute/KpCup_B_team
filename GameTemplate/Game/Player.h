#pragma once
class Game;
class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Rotation();
	void Move();
	//void PlayAnimation();



	ModelRender m_modelRender;
	Vector3 m_position;				//座標
	CharacterController m_charaCon;	//キャラクターコントローラー
	Vector3 m_moveSpeed;			//移動速度。
	Quaternion m_rotation;	//クォータニオン。
	Player* m_player = nullptr;


};

