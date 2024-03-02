#pragma once

struct Light
{
	//�f�B���N�V�������C�g
	Vector3 dirLigDirection;    // �f�B���N�V�������C�g�̕���
	float pad0;					// �p�f�B���O
	Vector3 dirColor;			// �f�B���N�V�������C�g�̃J���[
	float pad1;

	//�|�C���g���C�g
	Vector3 ptPosition;			// �ʒu
	float pd2;
	Vector3 ptColor;			// �J���[
	float ptRange;				// �e���͈�

	//�X�|�b�g���C�g
	Vector3 spPosition;
	float psd3;
	Vector3 spColor;
	float spRange;
	Vector3 spDirection;
	float spAngle;

	Vector3 eyePos;				// ���_�̈ʒu
	float pad4;

	//�������C�g
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

