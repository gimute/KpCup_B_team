#include "stdafx.h"
#include "Door.h"
#include "Player.h"

Door::Door()
{

}

Door::~Door()
{

}

namespace {
	const Vector3 corre1 = { 0.0f,60.0f,0.0f };
	const Vector3 colScale = { 135.0f,125.0f,135.0f };
	const float AddDoor = 1.5f;
}

bool Door::Start()
{
	//ドア枠
	m_RenderDoorMain.Init("Assets/modelData/door/doorframe.tkm");
	//位置など初期設定
	SetDoorMain(m_DoorMainPos, m_DoorMainRot, m_DoorMainSca);
	//ドア枠の当たり判定を作成
	m_DoorMainPhy.CreateFromModel(m_RenderDoorMain.GetModel(), m_RenderDoorMain.GetModel().GetWorldMatrix());
	//ドアL
	m_RenderDoorL.Init("Assets/modelData/door/doorLR.tkm");
	//位置など初期設定
	SetDoorL(m_DoorMainPos, m_DoorMainRot, m_DoorMainSca);
	//ドア左の当たり判定を作成
	m_DoorLPhy.CreateFromModel(m_RenderDoorL.GetModel(), m_RenderDoorL.GetModel().GetWorldMatrix());
	//ドアR
	m_RenderDoorR.Init("Assets/modelData/door/doorLR.tkm");
	//位置など初期設定
	SetDoorR(m_DoorMainPos, m_DoorMainRot, m_DoorMainSca);
	//ドア右の当たり判定を作成
	m_DoorRPhy.CreateFromModel(m_RenderDoorR.GetModel(), m_RenderDoorR.GetModel().GetWorldMatrix());

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateBox(m_DoorMainPos, m_DoorMainRot, colScale);
	m_collisionObject->SetName("door_col");
	m_collisionObject->SetPosition(m_DoorMainPos + corre1);
	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_player = FindGO<Player>("player");

	return true;
}

void Door::Update()
{
	//コリジョンの当たり判定処理
	Collision();
	//開閉
	OpenClose();
	// 当たり判定の座標を更新
	m_DoorMainPhy.GetRigitBody()->SetPositionAndRotation(m_DoorMainPos, m_DoorMainRot);
	m_DoorLPhy.GetRigitBody()->SetPositionAndRotation(m_DoorLPos, m_DoorLRot);
	m_DoorRPhy.GetRigitBody()->SetPositionAndRotation(m_DoorRPos, m_DoorRRot);
	//描画処理
	m_RenderDoorMain.Update();
	m_RenderDoorL.Update();
	m_RenderDoorR.Update();
}

void Door::Collision()
{
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_col");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_collisionObject))
		{
			//m_DoorOpen = true;
			return;
		}
	}
}

void Door::OpenClose()
{
	if (!m_DoorOpen)
		return;

	//最低速度に上限があってもいいかも
	Vector3 MovePowL = m_DoorLMove[1] - m_DoorLPos;
	MovePowL /= 70;
	float ratioL = m_DoorLMove[1].Length() / m_DoorLPos.Length();
	MovePowL = MovePowL * ratioL;
	Vector3 MovePowR = m_DoorRMove[1] - m_DoorRPos;
	MovePowR /= 70;
	float ratioR = m_DoorRMove[1].Length() / m_DoorRPos.Length();
	MovePowR = MovePowR * ratioR;
	if (MovePowL.Length() < 0.3f) {
		float test = MovePowL.Length();
	}

	if (m_DoorLPos.Length() < m_DoorLMove[1].Length())
	{
		m_DoorLPos += MovePowL;
		m_DoorRPos += MovePowR;
	}
	m_RenderDoorL.SetPosition(m_DoorLPos);
	m_RenderDoorR.SetPosition(m_DoorRPos);
}

void Door::Render(RenderContext& rc)
{
	m_RenderDoorMain.Draw(rc);
	m_RenderDoorL.Draw(rc);
	m_RenderDoorR.Draw(rc);
}