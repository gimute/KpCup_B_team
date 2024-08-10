#include "stdafx.h"
#include "Door.h"
#include "Player.h"
#include "Game.h"

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
	//プレイヤーのインスタンスを受け取る
	m_player = FindGO<Player>("player");
	//ゲームのインスタンスを受け取る
	m_game = FindGO<Game>("game");

	//文字を見えやすくするためにモヤモヤの画面を読み込む
	m_spriteRender_moya.Init("Assets/modelData/maintimer/moya.DDS", 700.0f, 100.0f);
	m_spriteRender_moya.SetPosition(Vector3{ 0.0f,-350.0f,0.0f });
	m_spriteRender_moya.SetMulColor({ 0.0f,0.0f,0.0f,0.4f });
	m_spriteRender_moya.Update();

	//アンダーバー画像の設定
	SpriteInitData initData;
	//DDSファイル（画像データ）のファイルパスを指定する
	//スタートボタンの画像データを指定する
	initData.m_ddsFilePath[0] = "Assets/modelData/maintimer/underBar.DDS";
	//Sprite表示用のシェーダーのファイルパスを指定する
	initData.m_fxFilePath = "Assets/shader/spriteUnder.fx";
	initData.m_expandConstantBuffer = &m_alpha;
	initData.m_expandConstantBufferSize += sizeof(float);
	//スプライトの幅と高さを指定する
	initData.m_width = static_cast<UINT>(25);
	initData.m_height = static_cast<UINT>(1.5);
	initData.m_alphaBlendMode = AlphaBlendMode_Trans;

	m_spRen_under.Init(initData);
	m_spRen_under.SetPosition(Vector3{ 150.0f,-368.0f,0.0f });
	//m_spRen_under.SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
	m_spRen_under.Update();

	//フォントレンダーの設定
	m_fontRender.SetText(L"敵をすべて倒さないと開かないようだ");
	m_fontRender.SetScale(0.35f);
	m_fontRender.SetPosition(Vector3(-350.0f, -400.0f, 0.0f));
	m_fontRender.SetColor({ 1.0f,1.0f,1.0f,1.0f });

	return true;
}

void Door::Update()
{
	//アルファチャンネルの調整
	AlphaCalc();
	m_spRen_under.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	m_spRen_under.Update();
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

void Door::AlphaCalc()
{
	if (m_alpha >= 1.0f)
	{
		m_alphaCalcBool = false;
	}
	if (m_alphaCalcBool)
	{
		m_alpha += 0.04f;
		return;
	}
	m_alpha -= 0.04f;
	if (m_alpha <= 0.1f)
	{
		m_alphaCalcBool = true;
	}
	
}

void Door::Collision()
{
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_col");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとコリジョンが衝突したら。
		if (collision->IsHit(m_collisionObject))
		{
			if (m_game->GetEnemyAllKillFlag())
			{
				m_DoorOpen = true;

				return;
			}
			else
			{
				m_spriteDraw = true;
				m_fontDraw = true;
				m_spUnderDraw = true;
				return;
			}
		}
	}
	m_spriteDraw = false;
	m_fontDraw = false;
	m_spUnderDraw = false;
}

void Door::OpenClose()
{
	if (m_DoorOpen == false)
	{
		return;
	}

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

	if ((m_DoorLPos - m_DoorLMove[1]).Length() >= 0.01f)
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

	if (m_fontDraw == true)
	{
		m_fontRender.Draw(rc);
	}
	if (m_spUnderDraw == true)
	{
		m_spRen_under.Draw(rc);
	}
	if (m_spriteDraw == true)
	{
		m_spriteRender_moya.Draw(rc);
	}
}