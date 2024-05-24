#pragma once
class Player;
class Door : public IGameObject
{
public:
	/////////////////////////////////////////関数
	Door();
	~Door();
	//スタート関数
	bool Start();
	//アップデート
	void Update();
	//当たり判定処
	void Collision();
	//開閉処理
	void OpenClose();
	//描画関数
	void Render(RenderContext& rc);
	/////////////////////////////////////////メンバ変数
	//ドア枠
	ModelRender m_RenderDoorMain;
	//右ドア
	ModelRender m_RenderDoorR;
	//左ドア
	ModelRender m_RenderDoorL;
	//プレイヤー
	Player* m_player = nullptr;
	//コリジョンオブジェクト
	CollisionObject* m_collisionObject;
	//ドア枠当たり判定
	PhysicsStaticObject m_DoorMainPhy;
	//ドア右当たり判定
	PhysicsStaticObject m_DoorRPhy;
	//ドア左当たり判定
	PhysicsStaticObject m_DoorLPhy;
	/////////////////////////////////////////変数
	//ドア枠pos
	Vector3 m_DoorMainPos = Vector3::Zero;
	//ドア右pos
	Vector3 m_DoorRPos = Vector3::Zero;
	//ドア左pos
	Vector3 m_DoorLPos = Vector3::Zero;
	//ドア枠rot
	Quaternion m_DoorMainRot;
	//ドア右rot
	Quaternion m_DoorRRot;
	//ドア左rot
	Quaternion m_DoorLRot;
	//ドア枠sca
	Vector3 m_DoorMainSca = { 1.0,1.0,1.0 };
	//ドア枠左ベクトル
	Vector3 m_DoorMainLeft = Vector3::AxisX * -1.0f;
	//ドア枠右ベクトル
	Vector3 m_DoorMainRight = Vector3::AxisX;
	//ドアナンバー
	int m_DoorNum = -1;
	//ドア左移動ベクトル配列
	Vector3 m_DoorLMove[2];
	//ドア右移動ベクトル配列
	Vector3 m_DoorRMove[2];
	//ドア開閉フラグ
	bool m_DoorOpen = false;
	/////////////////////////////////////////初期設定系統
	//位置設定
	void SetPosition(const Vector3& position)
	{
		m_DoorMainPos = position;
		return;
	}
	//回転設定
	void SetRotation(const Quaternion& rotation)
	{
		m_DoorMainRot = rotation;
		return;
	}
	//大きさ設定
	void SetScale(const Vector3& scale)
	{
		m_DoorMainSca = scale;
		return;
	}
	//ドアナンバー設定
	void SetDoorNumber(const int num)
	{
		m_DoorNum = num;
		return;
	}
	void SetDoorMain(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
	{
		m_RenderDoorMain.SetPosition(position);
		m_RenderDoorMain.SetRotation(rotation);
		m_RenderDoorMain.SetScale(scale);
	}
	void SetDoorL(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
	{
		m_DoorMainLeft = Vector3::AxisX * -1.0f;
		m_DoorMainRot.Apply(m_DoorMainLeft);
		m_DoorLRot = rotation;
		m_RenderDoorL.SetRotation(m_DoorLRot);
		m_DoorLPos = m_DoorMainLeft;
		m_DoorLPos.Normalize();
		m_DoorLPos *= 30.0f;
		m_DoorLPos += position;
		m_RenderDoorL.SetPosition(m_DoorLPos);
		m_RenderDoorL.SetScale(scale);
		//移動配列の設定
		//移動前、原点
		m_DoorLMove[0] = m_DoorLPos;
		//移動先
		Vector3 Left = Vector3::AxisX * -1.0f;
		m_DoorMainRot.Apply(Left);
		Left.Normalize();
		Left *= 60.0f;
		Left += m_DoorLMove[0];
		m_DoorLMove[1] = Left;
		return;
	}
	void SetDoorR(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
	{
		m_DoorMainRight = Vector3::AxisX;
		m_DoorMainRot.Apply(m_DoorMainRight);
		m_DoorRRot = rotation;
		m_RenderDoorR.SetRotation(m_DoorRRot);
		m_DoorRPos = m_DoorMainRight;
		m_DoorRPos.Normalize();
		m_DoorRPos *= 30.0f;
		m_DoorRPos += position;
		m_RenderDoorR.SetPosition(m_DoorRPos);
		m_RenderDoorR.SetScale(scale);
		//移動配列の設定
		//移動前、原点
		m_DoorRMove[0] = m_DoorRPos;
		//移動先
		Vector3 Right = Vector3::AxisX;
		m_DoorMainRot.Apply(Right);
		Right.Normalize();
		Right *= 60.0f;
		Right += m_DoorRMove[0];
		m_DoorRMove[1] = Right;
		return;
	}
};

