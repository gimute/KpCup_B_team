#pragma once
class MainTimer : public IGameObject
{
public:
	/////////////////////////////////////////関数
	MainTimer() {}
	~MainTimer() {}
	//スタート関数
	bool Start();
	//アップデート
	void Update();
	//フォントドロー
	void FontDrow();
	//タイム減少処理
	void ReductionTimer();
	//タイム位置調整処理
	void TimerPositionUpdate();
	//レンダー
	void Render(RenderContext& rc);
	/////////////////////////////////////////メンバ変数
	//フォントレンダー
	FontRender m_fontrender;
	//スプライトレンダー
	SpriteRender m_spriterender;
	/////////////////////////////////////////変数
private:
	//現在タイマー値
	int m_timer = 0.0f;
	//1秒待つ時に使うタイマー
	float m_1timer = 0;
	const float m_1time = 1.0;
	//最大タイマー値
	const float m_fulltimer = 0.0f;
	//タイマーを減らすか減らさないか
	//trueで減らす
	bool m_reductiontime_bool = false;
	//時間位置
	Vector3 m_timerposition = { -900.0f,-320.0f,0.0f };
	//時間大きさ
	float m_timerscale = 3.5f;
	//時間色
	Vector4 m_timercolor = { 0.0f,0.0f,0.0f,1.0f };
	//UI位置
	Vector3 m_uiposition = { -630.0f,-330.0f,0.0f };
	/////////////////////////////////////////初期設定系統
public:
	//タイマーセット
	void SetTimer(int settime) {
		m_timer = settime;
		m_reductiontime_bool = true;
		return;
	}
	//タイマープラス
	void purasuTimer(int addtime) {
		m_timer += addtime;
		m_1timer = m_1time;
		return;
	}
	//タイマーマイナス
	void mainasuTimer(int reductiontime) {
		m_timer -= reductiontime;
		m_1timer = m_1time;
		return;
	}
	//時間位置設定
	void SetTimerPosition(const Vector3& position)
	{
		m_timerposition = position;
		return;
	}
	//時間大きさ設定
	void SetTimerScale(float scale)
	{
		m_timerscale = scale;
		return;
	}
	//時間色設定
	void SetTimerColor(const Vector4& color)
	{
		m_timercolor = color;
		return;
	}
	//時間UI位置設定
	void SetUIPosition(const Vector3& position)
	{
		m_uiposition = position;
		return;
	}
	void SetUIScale(const Vector3& scale)
	{

	}
};

