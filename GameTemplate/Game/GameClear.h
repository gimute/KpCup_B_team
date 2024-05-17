#pragma once
#include "Game.h"

class GameClear : public IGameObject
{
public:
	GameClear();
	~GameClear();
	//�X�V����
	void Update();
	//�`�揈��
	void Render(RenderContext& rc);
	
	//�����o�ϐ�
	SpriteRender m_spriterender;	//�X�v���C�g�����_�[

	Game* m_game = nullptr;
	FontRender m_fontRender;
	GameClear* m_gameclear;
};

