#pragma once

class Game;
class Load;
class GameOver : public IGameObject 
{
public:
	GameOver();
	~GameOver();
	bool Start();
	//�X�V����
	void Update();
	//�`�揈��
	void Render(RenderContext& rc);

	Game* m_game = nullptr;
	Load* m_load = nullptr;

	//�����o�ϐ�
	SpriteRender m_spriteRender;	//�X�v���C�g�����_�[
	bool m_isWaitFadeout = false;
};

