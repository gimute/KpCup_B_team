#include "stdafx.h"
#include "ModelTest.h"

bool ModelTest::Start()
{
	//ライトの初期化///////////////////////

	//ディレクションライト
	m_light.dirLigDirection.x = 1.0f;
	m_light.dirLigDirection.y = -1.0f;
	m_light.dirLigDirection.z = -1.0f;

	m_light.dirLigDirection.Normalize();

	m_light.dirColor.x = 1.0f;
	m_light.dirColor.y = 1.0f;
	m_light.dirColor.z = 1.0f;

	m_light.eyePos = g_camera3D->GetPosition();

	//ポイントライト
	m_light.ptPosition.x = 0.0f;
	m_light.ptPosition.y = 50.0f;
	m_light.ptPosition.z = 0.0f;

	m_light.ptColor.x = 15.0f;
	m_light.ptColor.y = 0.0f;
	m_light.ptColor.z = 0.0f;

	m_light.ptRange = 100.0f;

	//スポットライト
	m_light.spPosition.x = 0.0f;
	m_light.spPosition.y = 50.0f;
	m_light.spPosition.z = 0.0f;

	m_light.spColor.x = 10.0f;
	m_light.spColor.y = 10.0f;
	m_light.spColor.z = 10.0f;

	m_light.spDirection.x = 1.0f;
	m_light.spDirection.y = -1.0;
	m_light.spDirection.z = 1.0f;
	m_light.spDirection.Normalize();

	m_light.spRange = 300.0f;

	m_light.spAngle = Math::DegToRad(25.0f);

	//半球ライト
	m_light.groundColor.x = 0.7f;
	m_light.groundColor.y = 0.5f;
	m_light.groundColor.z = 0.3f;

	m_light.skyColor.x = 0.15f;
	m_light.skyColor.y = 0.7f;
	m_light.skyColor.z = 0.95f;

	m_light.groundNormal.x = 0.0f;
	m_light.groundNormal.y = 1.0f;
	m_light.groundNormal.z = 0.0f;
	//一応正規化
	m_light.groundNormal.Normalize();

	////////////////////////////////////

	// モデルを初期化するための情報を設定。
	ModelInitData initData;
	//tkmファイルのファイルパスを指定する。
	initData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = "Assets/shader/test.fx";

	initData.m_expandConstantBuffer = &m_light;
	initData.m_expandConstantBufferSize = sizeof(m_light);

	//作成した初期化データをもとにモデルを初期化する、
	m_model.Init(initData);


	//bgモデル
	ModelInitData bgModelInitData;
	bgModelInitData.m_tkmFilePath = "Assets/modelData/bg.tkm";
	bgModelInitData.m_fxFilePath = "Assets/shader/test.fx";

	bgModelInitData.m_expandConstantBuffer = &m_light;
	bgModelInitData.m_expandConstantBufferSize = sizeof(m_light);

	m_bgModel.Init(bgModelInitData);

	return true;
}

void ModelTest::Update()
{

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_moveState++;

		m_moveState %= 4;
	}

	switch (m_moveState)
	{
	case 0:
		ModelMove();
		break;

	case 1:
		DirLigMove();
		break;

	case 2:
		PtLigMove();
		break;

	case 3:
		SpotLigMove();
		break;

	default:
		break;
	}

}

void ModelTest::Render(RenderContext& rc)
{
	m_model.Draw(rc);
	m_bgModel.Draw(rc);
}

void ModelTest::ModelMove()
{
	//モデルの移動
	m_position.x += g_pad[0]->GetLStickXF();
	m_position.y += g_pad[0]->GetLStickYF();
	//モデルの回転
	m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
	m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);

	//どちらもカメラ基準の処理にしていない
	if (g_pad[0]->IsPress(enButtonUp)) {
		m_scale.y += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonDown)) {
		m_scale.y -= 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_scale.x += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_scale.x -= 0.02f;
	}

	//モデルのワールド行列更新
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}

void ModelTest::DirLigMove()
{
	//ディレクションライトの向きの回転
	Quaternion qRot;
	qRot.SetRotationDegY(g_pad[0]->GetLStickXF());
	qRot.Apply(m_light.dirLigDirection);
	qRot.SetRotationDegX(g_pad[0]->GetLStickYF());
	qRot.Apply(m_light.dirLigDirection);
}

void ModelTest::PtLigMove()
{
	m_light.ptPosition.x -= g_pad[0]->GetLStickXF();
	if (g_pad[0]->IsPress(enButtonB))
	{
		m_light.ptPosition.y += g_pad[0]->GetLStickYF();
	}
	else
	{
		m_light.ptPosition.z -= g_pad[0]->GetLStickYF();
	}
}

void ModelTest::SpotLigMove()
{
	m_light.spPosition.x -= g_pad[0]->GetLStickXF();
	if (g_pad[0]->IsPress(enButtonB))
	{
		//Bボタンが一緒に押されていたらY軸方向に動かす
		m_light.spPosition.y += g_pad[0]->GetLStickYF();
	}
	else
	{
		//Z軸方向に動かす
		m_light.spPosition.z -= g_pad[0]->GetLStickYF();
	}

	// step-4 コントローラー右スティックでスポットライトを回転させる
	//Y軸周りの回転クォータニオンを計算する
	Quaternion qRotY;
	qRotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.01f);

	//計算したクォータニオンデライトの方向を回す
	qRotY.Apply(m_light.spDirection);

	//X軸周りの回転クォータニオンを計算する
	Vector3 rotAxis;
	rotAxis.Cross(g_vec3AxisY, m_light.spDirection);
	Quaternion qRotX;
	qRotX.SetRotation(rotAxis, g_pad[0]->GetRStickYF() * 0.01f);

	//計算したクォータニオンデライトの方向を回す
	qRotX.Apply(m_light.spDirection);
}
