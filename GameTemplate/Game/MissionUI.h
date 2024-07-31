#pragma once

class Game;

class MissionUI : public IGameObject
{
public:
	//ミッションステート
	enum EnMissionState
	{
		enEnemyOllKill,	//敵を殲滅
		enGoDoor,		//ドアへ向かう
		enNone			//無し
	};

	MissionUI();
	~MissionUI();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//UIのポジションを設定
	void SetPosition(Vector3 ps)
	{
		m_position = ps;
		m_updateFlag = true;
	}

	void SetDrawFlag(bool flag)
	{
		m_DrawFlag = flag;
	}

private:
	SpriteRender m_missionSprite;		//ミッションフレーム
	FontRender m_missionString;			//ミッション文
	Vector3 m_position = Vector3::Zero;	//UIのポジション
	int m_nowEnemyCount = 0;			//現在の敵の数
	int m_defeatEnemyNumber = 0;		//倒す敵の数
	Game* m_game = nullptr;				//ゲーム
	EnMissionState m_missionState = enNone;	//ミッションステート
	bool m_updateFlag = false;			//アップデート処理を行うかのフラグ
	bool m_DrawFlag = true;				//描画するかどうかのフラグ
};

