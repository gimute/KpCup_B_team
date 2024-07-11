#include "stdafx.h"
#include "Bullet.h"

//作る時はこんな感じで↓
//if (g_pad[0]->IsTrigger(enButtonB))
//{
//	m_bullet = NewGO<Bullet>(0, "bullet");
//	m_bullet->SetMoveDirection(m_forward);
//	m_bullet->Setrotation(m_rotation);
//	m_bullet->SetPosition(m_position);
//}

//WallCheckに使っている構造体、WallCheckの位置を動かすときは一緒に動かしてください
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& covexResult, bool normalInWorldSpace)
	{
		//壁とぶつかっていなかったら
		if (covexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall)
		{
			//衝突したのは壁ではない
			return 0.0f;
		}

		//壁とぶつかったらフラグをtrueにする
		isHit = true;
		return 0.0f;
	}
};

namespace {
	//弾丸消去ディレイタイマー
	const float deletetimer = 0.1f;
	//大きさ
	const Vector3 scale = { 1.5f,1.5f,1.5f };
	//位置修正
	const Vector3 corre = { 0.0f,55.0f,0.0f };
}

bool Bullet::Start()
{
	m_modelrender.Init("Assets/modelData/bullet/bullet_proto.tkm");

	m_rotation.Apply(m_velocity);
	m_position.x += m_velocity.x * 20.0f;
	m_position.z += m_velocity.z * 20.0f;
	m_position.y += m_velocity.y * 70.0f;
	m_velocity *= 1200.0f;
	m_position += corre;

	
	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 20.0f * m_scale.z);
	if (m_shotType == en_Player)
	{
		m_collisionObject->SetName("player_attack");
	}
	else
	{
		m_collisionObject->SetName("enemy_attack");
	}
	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);
	
	return true;
}

void Bullet::Update()
{
	//弾丸移動
	Movebullet();
	//回転処理
	Rotation();
	//弾丸時間消滅処理
	Inpacttime();
	//弾丸対象衝突処理
	Inpacthit();
	//弾丸消去処理
	deletebullet();

	//次の移動先となる座標を計算する。
	Vector3 nextPosition = m_position;
	//速度からこのフレームでの移動量を求める。オイラー積分。
	Vector3 addPos = m_velocity * 0.8f;
	addPos.Scale(g_gameTime->GetFrameDeltaTime());
	nextPosition.Add(addPos);

	m_sphereCollider.Create(0.5f);

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	//始点は弾丸の基点
	start.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
	end.setOrigin(btVector3(nextPosition.x, m_position.y, nextPosition.z));
	SweepResultWall callback;

	//衝突検出。
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	if (callback.isHit) {
		//当たった。
		DeleteGO(m_collisionObject);
		DeleteGO(this);
	}
	//描画処理
	m_modelrender.Update();
}

void Bullet::Movebullet()
{
	//座標を移動させる。
	m_position += m_velocity * g_gameTime->GetFrameDeltaTime();
	m_modelrender.SetPosition(m_position);
	m_collisionObject->SetPosition(m_position);
	bullettime -= g_gameTime->GetFrameDeltaTime();	//自然消去タイマーを減らすヤツ
}

void Bullet::Rotation()
{
	//正確には弾丸を移動方向に向かせるプログラム
				//移動速度を↓に入れるとできる。
	float angle = atan2(-m_velocity.x, m_velocity.z);
	m_rotation.SetRotationY(-angle);
	m_modelrender.SetRotation(m_rotation);
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Bullet::Inpacttime()
{
	if (bullettime > 0)
		return;
	DeleteGO(m_collisionObject);
	DeleteGO(this);
}

void Bullet::Inpacthit()
{

	if (m_collisionObject->GetName() == "player_attack") {
		//enemyのコリジョンを取得する												//↓enemyの共通コリジョン
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_col");
		//コリジョンの配列をfor文で回す
		for (auto collision : collisions)
		{
			//コリジョンとキャラコンが衝突したら
			if (collision->IsHit(m_collisionObject))
			{
				if (m_isDelete == false) {
					//deletebulletのif文が通るようにする
					m_isDelete = true;
					//deletetimerは現在0.2f
					m_deleteTimer = deletetimer;
				}
			}
		}
	}

	if (m_collisionObject->GetName() == "enemy_attack") {
		//playerのコリジョンを取得する
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_col");
		//コリジョンの配列をfor文で回す
		for (auto collision : collisions)
		{
			//コリジョンとキャラコンが衝突したら
			if (collision->IsHit(m_collisionObject))
			{
				if (m_isDelete == false) {
					//deletebulletのif文が通るようにする
					m_isDelete = true;
					//deletetimerは現在0.2f
					m_deleteTimer = deletetimer;
				}
			}
		}
	}

}

void Bullet::deletebullet()
{
	if (m_isDelete)
	{

		m_deleteTimer -= g_gameTime->GetFrameDeltaTime(); //deletetimerを1フレームずつ
		//減らす。
			//↓タイマーがゼロになったら。(deletetimerより0の方が大きくなったら)
		if (m_deleteTimer <= 0.0f)
		{
			DeleteGO(m_collisionObject);
			DeleteGO(this);
		}
	}
}

void Bullet::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
}
