#pragma once
class GameOver : public IGameObject 
{
public:
	GameOver();
	~GameOver();
	//�X�V����
	void Update();
	//�`�揈��
	void Render(RenderContext& rc);

	//�����o�ϐ�
	SpriteRender m_spriteRender;	//�X�v���C�g�����_�[
};

