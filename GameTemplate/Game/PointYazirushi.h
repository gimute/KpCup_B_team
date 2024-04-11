#pragma once
class Player;
class PointYazirushi : public IGameObject
{
public:
	/////////////////////////////////////////関数
	PointYazirushi() {}
	~PointYazirushi() {}
	//スタート関数
	bool Start();
	//アップデート関数
	void Update();
	//位置更新
	void PositionUpdate();
	//回転処理
	void Rotation();
	//レンダー
	void Render(RenderContext& rc);
	/////////////////////////////////////////メンバ変数
	//モデルレンダー
	ModelRender m_modelrender;
	//座標
	Vector3 m_position;
	//大きさ
	Vector3 m_scale = Vector3::One;
	//回転
	Quaternion m_rotation;
	//正面ベクトル
	Vector3	m_forward = Vector3::AxisZ;
	//プレイヤー
	Player* m_player = nullptr;
	/////////////////////////////////////////変数
	//やじ対象位置
	Vector3 m_targetpos = { 0.0,0.0,0.0 };
	//座標更新をするか
	bool m_positionupdate = false;
	/////////////////////////////////////////初期設定系統
public:
	//位置設定
	void SetPosition(const Vector3& position)
	{
		m_position = position;
		return;
	}
	//矢印ターゲット設定
	void SetTarget(const Vector3& position)
	{
		m_targetpos = position;
		return;
	}
	//回転設定
	void SetRotation(const Quaternion& rotation) {
		m_rotation = rotation;
		return;
	}
	//大きさ設定
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		return;
	}
};

