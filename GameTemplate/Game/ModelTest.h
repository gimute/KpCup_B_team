#pragma once
class ModelTest : public IGameObject
{
public:
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
		float specPow;				//�X�y�L�����̍i��

		//�������C�g
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
	Model m_model;					//���f��
	Model m_bgModel;
	Light m_light;					//�V�F�[�_�ɑ��郉�C�g�̏��
	Vector3 m_position;				//���W
	Quaternion m_rotation;			//��]
	Vector3 m_scale = g_vec3One;	//�g�嗦


	int m_moveState = 0;
};

