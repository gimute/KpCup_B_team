#pragma once
class Car : public IGameObject
{
public: 
	Car();
	~Car();
	bool Start();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);


	//メンバ変数。
	ModelRender				m_modelRender;			//モデルレンダー。
	Vector3					m_position;				//座標。
	Vector3					m_scale = Vector3::One;	//大きさ。
	PhysicsStaticObject		physicsStaticObject;	//静的オブジェクトを保持させる。

};

