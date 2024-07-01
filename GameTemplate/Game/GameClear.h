#pragma once
#include "sound/SoundSource.h"
class Game;
class GameTimer;
class Load;
class GameClear : public IGameObject
{
public:
	GameClear();
	~GameClear();
	bool Start();
	void AlphaCalc();
	//�X�V����
	void Update();
	//�N���A�^�C���\��
	void DisplayTime();
	//�`�揈��
	void Render(RenderContext& rc);
	
	//�����o�ϐ�
	SpriteRender m_spriterender;	//�X�v���C�g�����_�[
	SpriteRender m_spriterender_moya;	//�X�v���C�g�����_�[
	SpriteRender m_pressRender;		//Press A Button �����_�[
	Game* m_game = nullptr;
	GameTimer* m_gametimer = nullptr;
	GameClear* m_gameclear;
	Load* m_load = nullptr;
	FontRender m_fontRender;
	FontRender m_fontRender_tensen;
	SoundSource* m_clearBgm;

	bool m_isWaitFadeout = false;
	bool m_isSaveClearTime = false;
	float m_clearminit = 0.0f;
	int m_cleartimer = 0.0f;

	//�A���t�@�`�����l���̒����p�̕ϐ�
	float m_alpha = 0.1f;
	bool m_alphaCalcBool;
};

