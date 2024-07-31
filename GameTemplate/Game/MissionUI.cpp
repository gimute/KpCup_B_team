#include "stdafx.h"
#include "MissionUI.h"
#include "Game.h"

namespace {
	//ミッションのフレーム用定数
	Vector2 MISSION_FRAME_SIZE = { 480.0f,44.0f };		//ミッションフレームのサイズ
	Vector3 MISSION_FRAME_SCALE = { 0.8f,1.0f,1.0f };	//ミッションフレームのスケール
	Vector2 MISSION_FRAME_PIVOT = { 0.0f,0.0f };		//ミッションフレームのピボット
														//ゲームタイマーの右下にくっつけるのでピボットは左下に設定
	Vector4 MISSION_MUL_COLOR = { 1.5f,1.5f,1.5f,1.0f };//ミッションフレームの乗算カラー

	//ミッション文用定数
	Vector3 MISSION_STRING_POSITION = { 0.0f,50.0f,0.0f };		//文の位置をミッションフレームに合わせるためのVector
	float MISSION_STRING_SCALE = 0.4f;							//文のスケール
	Vector4 MISSION_STRING_COLOR = { 0.68f,0.68f,0.68f,1.0f };	//文字の色
	float MISSION_STRING_SHADOW_OFFSET = 0.5;					//文字の影のオフセット、影は文字を太くするために使っている
}

MissionUI::MissionUI()
{
	
}

MissionUI::~MissionUI()
{

}

bool MissionUI::Start()
{
	//最初のミッションステートを設定
	m_missionState = enEnemyOllKill;

	//ミッションフレームの諸々設定
	m_missionSprite.Init("Assets/sprite/MissionUI/missionUIFrame.DDS", MISSION_FRAME_SIZE.x, MISSION_FRAME_SIZE.y);
	m_missionSprite.SetScale(MISSION_FRAME_SCALE);
	m_missionSprite.SetPivot(MISSION_FRAME_PIVOT);
	m_missionSprite.SetMulColor(MISSION_MUL_COLOR);
	m_missionSprite.Update();

	//ミッション文諸々設定
	m_missionString.SetPosition(MISSION_STRING_POSITION);
	m_missionString.SetScale(MISSION_STRING_SCALE);
	m_missionString.SetColor(MISSION_STRING_COLOR);
	//文字が細かったので文字と同じ色の影を付けて無理やり太くする
	m_missionString.SetShadowParam(true, MISSION_STRING_SHADOW_OFFSET, MISSION_STRING_COLOR);

	//ゲームのインスタンスのポインタを受け取る
	m_game = FindGO<Game>("game");

	//このクラスのオブジェクトを作った時の敵の数を、倒す敵の数として設定する
	m_defeatEnemyNumber = m_game->GetEnemyListSize();
	//一応現在の敵の数も代入しておく
	m_nowEnemyCount = m_defeatEnemyNumber;

	return true;
}

void MissionUI::Update()
{
	//アップデートフラグがturuなら
	if (m_updateFlag)
	{
		//スプライトのポジションを設定
		m_missionSprite.SetPosition(m_position);
		m_missionSprite.Update();

		//ミッション文のポジションを設定
		m_missionString.SetPosition(MISSION_STRING_POSITION + (m_position * 1.2f));
	}

	switch (m_missionState)
	{
	case MissionUI::enEnemyOllKill:
		//現在の敵の数を取得
		m_nowEnemyCount = m_game->GetEnemyListSize();

		//敵が居なくなったら
		if (m_nowEnemyCount <= 0)
		{
			//ミッションステートをドアに向かえに変更
			m_missionState = enGoDoor;
		}

		//missionStringに入れる文の準備
		wchar_t wcsbuf[256];
		swprintf_s(wcsbuf, 256, L" 敵を全て倒せ  残り%02d/%02d", int(m_nowEnemyCount), int(m_defeatEnemyNumber));

		//ミッション文を設定
		m_missionString.SetText(wcsbuf);
		break;

	case MissionUI::enGoDoor:
		//ミッション文を設定
		m_missionString.SetText(L" ドアに向かえ");
		break;

	case MissionUI::enNone:
		break;

	default:
		break;
	}
}

void MissionUI::Render(RenderContext& rc)
{
	if (m_DrawFlag)
	{
		m_missionSprite.Draw(rc);
		m_missionString.Draw(rc);
	}
}
