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

private:
	SpriteRender m_missionSprite;		//ミッション画像
	FontRender m_mission;				//ミッション

	Game* m_game = nullptr;				//

	EnMissionState m_missionState = enNone;

	int m_nowEnemyCount = 0;			//現在の敵の数
	int m_maxEnemyCount = 0;			//元々いた敵の数

	Vector3 m_position = Vector3::Zero;	//UIのポジション

	bool m_updateFlag = false;			//諸々のアップデート処理を行うかのフラグ
};

