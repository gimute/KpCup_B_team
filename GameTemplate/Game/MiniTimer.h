#pragma once
class MiniTimer : public IGameObject
{
public:
	/////////////////////////////////////////関数
	MiniTimer() {}
	~MiniTimer() {}
	//スタート関数
	bool Start();
	//アップデート
	void Update();
	//フォントドロー
	void FontDrow();
	//タイム減少処理
	void ReductionTimer();
	//レンダー
	void Render(RenderContext& rc);
	/////////////////////////////////////////メンバ変数
	FontRender m_fontrender;				//スプライトレンダー
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
	//位置
	Vector3 m_position = { 0.0f,0.0f,0.0f };
	//大きさ
	float m_scale = 1.0f;
	//色
	Vector4 m_color = { 0.0f,0.0f,0.0f,1.0f };
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
		return;
	}
	//タイマーマイナス
	void mainasuTimer(int reductiontime) {
		m_timer -= reductiontime;
		return;
	}
	//位置設定
	void SetPosition(const Vector3& position)
	{
		m_position = position;
		return;
	}
	//大きさ設定
	void SetScale(float scale)
	{
		m_scale = scale;
		return;
	}
	//色設定
	void SetColor(const Vector4& color)
	{
		m_color = color;
		return;
	}
};

