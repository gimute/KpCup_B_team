#pragma once
#include "sound/SoundSource.h"
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
	//���ߏ���
	void AlphaCalc();

	Game* m_game = nullptr;
	Load* m_load = nullptr;

	//�����o�ϐ�
	SpriteRender m_spriteRender;	//�X�v���C�g�����_�[
	SpriteRender m_pressRender;
	SoundSource* m_overBgm;			//�Q�[���I�[�o�[��BGM
	bool m_isWaitFadeout = false;
	float m_alpha = 0.0f;
	bool m_alphaCalcBool = false;
};

