#pragma once
class ModelTest : public IGameObject
{
public:
	struct Light
	{
		//ディレクションライト
		Vector3 dirLigDirection;    // ディレクションライトの方向
		float pad0;					// パディング
		Vector3 dirColor;			// ディレクションライトのカラー
		float pad1;

		//ポイントライト
		Vector3 ptPosition;			// 位置
		float pd2;
		Vector3 ptColor;			// カラー
		float ptRange;				// 影響範囲

		//スポットライト
		Vector3 spPosition;
		float psd3;
		Vector3 spColor;
		float spRange;
		Vector3 spDirection;
		float spAngle;

		Vector3 eyePos;				// 視点の位置
		float specPow;				//スペキュラの絞り

		//半球ライト
		Vector3 groundColor;
		float pad5;
		Vector3 skyColor;
		float pad6;
		Vector3 groundNormal;

	};

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void ModelMove();
	void DirLigMove();
	void PtLigMove();
	void SpotLigMove();

private:
	Model m_model;					//モデル
	Model m_bgModel;
	Light m_light;					//シェーダに送るライトの情報
	Vector3 m_position;				//座標
	Quaternion m_rotation;			//回転
	Vector3 m_scale = g_vec3One;	//拡大率


	int m_moveState = 0;
};

