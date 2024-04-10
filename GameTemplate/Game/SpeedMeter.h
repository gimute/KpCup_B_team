#pragma once
class Player;
class SpeedMeter : public IGameObject
{
public:
	SpeedMeter();
	~SpeedMeter();

	bool Start();
	//更新処理。
	void Update();

	void Render(RenderContext& rc);

//メンバ変数
	FontRender m_fontrender;	//フォントレンダー
	Player* m_player;
};

