#include "stdafx.h"
#include "PointYazirushi.h"
#include "Player.h"

namespace {
	const Vector3 correction = { 0.0,250.0,0.0 };
}

bool PointYazirushi::Start()
{
	//モデルを設定
	m_modelrender.Init("Assets/modelData/yazirushi/yazirushi.tkm");
	//位置設定
	m_modelrender.SetPosition(m_position);
	//回転設定
	m_modelrender.SetRotation(m_rotation);
	//大きさ設定
	m_modelrender.SetScale(m_scale);
	//プレイヤー読み込み
	m_player = FindGO<Player>("player");
	//位置更新開始
	m_positionupdate = true;
	return true;
}

void PointYazirushi::Update()
{
	//位置更新
	PositionUpdate();
	//回転処理
	Rotation();
	//ドローコール
	m_modelrender.Update();
}

void PointYazirushi::PositionUpdate()
{
	if (m_positionupdate == true)
	{
		//位置を少し上げて
		Vector3 pos = correction + m_player->m_position;
		//位置更新
		m_modelrender.SetPosition(pos);
	}
}

void PointYazirushi::Rotation()
{
	if (m_positionupdate == true)
	{
		//やじ対象座標-プレイヤー座標を計算して、プレイヤー座標からやじ対象へのベクトルを出す
		Vector3 diff = m_targetpos - m_player->m_position;
		//向きだけが欲しいので単位ベクトル化
		diff.Normalize();
		//更新など
		m_rotation.SetRotation(m_forward, diff);
		m_modelrender.SetRotation(m_rotation);
	}
}

void PointYazirushi::Render(RenderContext& rc)
{
	m_modelrender.Draw(rc);
}