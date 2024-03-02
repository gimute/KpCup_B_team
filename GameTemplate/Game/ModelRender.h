#pragma once

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
	float pad4;

	//半球ライト
	Vector3 groundColor;
	float pad5;
	Vector3 skyColor;
	float pad6;
	Vector3 groundNormal;

};

class ModelRender
{
public:
	ModelRender();
	~ModelRender();

	void init();

	void Render(RenderContext& rc);

private:
	Model m_model;
};

