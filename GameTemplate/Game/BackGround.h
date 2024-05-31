#pragma once
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();

	bool Start();
	void Update();
	void Render(RenderContext& rc);


//メンバ関数

	ModelRender				m_modelRender;			//モデルレンダー。
	ModelRender				m_floor;				//床用モデルレンダー
	Vector3					m_position;				//座標。
	Vector3					m_scale = Vector3::One;	//大きさ。
	PhysicsStaticObject		physicsStaticObject;	//静的オブジェクトを保持させる。
	PhysicsStaticObject		physicsStaticObject_floor;	//静的オブジェクトを保持させる。床用
};

