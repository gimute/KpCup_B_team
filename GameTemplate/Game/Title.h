#pragma once

// �^�C�g���B
class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	//�`��֐��B
	void Render(RenderContext& rc);
	//�X�V�����B
	void Update();
	//�A���t�@�`�����l������
	void AlphaCalc();

	//�X�v���C�g�����_�[�B
	SpriteRender m_spriteRender;
	SpriteRender m_spriteRender2;
	
	//�A���t�@�`�����l���̒����p�̕ϐ�
	float m_alpha = 0.1f;
	bool m_alphaCalcBool;
};

