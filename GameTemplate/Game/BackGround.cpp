#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{

}

BackGround::~BackGround()
{

}

bool BackGround::Start()
{
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/Map/Map6.tkm", nullptr, 0, enModelUpAxisZ, true);
	//座標を設定する。
	m_modelRender.SetPosition(m_position);
	//大きさを設定する。
	m_modelRender.SetScale(m_scale);

	//モデルを更新する。
	m_modelRender.Update();
	//静的物理オブジェクトを作成
	physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	//モデルを読み込む
	m_floor.Init("Assets/modelData/Map/Map_floor.tkm");
	//座標を設定する。
	m_floor.SetPosition(m_position);
	//大きさを設定する。
	m_floor.SetScale(m_scale);

	//モデルを更新する。
	m_floor.Update();
	//静的物理オブジェクトを作成
	physicsStaticObject_floor.CreateFromModel(m_floor.GetModel(), m_floor.GetModel().GetWorldMatrix());

	// 当たり判定を可視化する。
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	physicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);
	physicsStaticObject.GetRigitBody();
	return true;
}

void BackGround::Update()
{
	//モデルを更新する。
	m_modelRender.Update();
}

void BackGround::Render(RenderContext& rc)
{
	//モデルを描画する。
	m_modelRender.Draw(rc);
	m_floor.Draw(rc);
}
