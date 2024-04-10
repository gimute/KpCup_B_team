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
	Vector3					m_position;				//座標。
	Vector3					m_scale = Vector3::One;	//大きさ。
	PhysicsStaticObject		physicsStaticObject;	//静的オブジェクトを保持させる。
};

