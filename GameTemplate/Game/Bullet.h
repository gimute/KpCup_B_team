#pragma once
class Bullet : public IGameObject
{
public:
	/////////////////////////////////////////関数
	Bullet() {};
	~Bullet() {};
	bool Start();
	//アップデート
	void Update();
	//回転
	void Rotation();
	//弾丸移動
	void Movebullet();
	//モデルレンダー
	void Render(RenderContext& rc);
	//弾丸自然消去
	void Inpacttime();
	//弾丸衝突
	void Inpacthit();
	//弾丸消去
	void deletebullet();
	/////////////////////////////////////////メンバ変数
	//モデルレンダ―
	ModelRender m_modelrender;
	//コリジョンオブジェクト
	CollisionObject* m_collisionObject;
	/////////////////////////////////////////変数
	//正面ベクトル
	Vector3 m_forward = Vector3::AxisZ;
	//速度
	Vector3 m_velocity = Vector3::AxisZ;
	//進行方向
	Vector3 m_direction = Vector3::Zero;
	//位置
	Vector3 m_position = Vector3::Zero;
	//回転
	Quaternion m_rotation;
	//大きさ
	Vector3	m_scale = Vector3::One;
	//自然消去タイマー
	float bullettime = 1.5f;
	//消去ディレイ
	float m_deleteTimer = 0.0f;
	bool m_isDelete = false;
	/////////////////////////////////////////初期設定系統
	//進行方向設定(プレイヤーの正面ベクトルなど)
	void SetMoveDirection(const Vector3 direction)
	{
		m_direction = direction;
		return;
	}
	//位置設定
	void SetPosition(const Vector3 position)
	{
		m_position = position;
		return;
	}
	//回転設定
	void Setrotation(const Quaternion& rotation)//回転値初期設定
	{
		m_rotation = rotation;
	}
};

