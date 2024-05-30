#pragma once

class Load;
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
	SpriteRender m_spriteRender_title;
	SpriteRender m_spriteRender_startButton;
	
	Load* m_load = nullptr;
	bool m_isWaitFadeout = false;

	//�A���t�@�`�����l���̒����p�̕ϐ�
	float m_alpha = 0.1f;
	bool m_alphaCalcBool;
};

