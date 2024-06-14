#pragma once
class Game;
class GameClear;

class GameTimer:public IGameObject
{
public:
	GameTimer();
	~GameTimer();

	bool Start();
	//�X�V����
	void Update();
	//�^�C���o�ߏ���
	void IncreaseTimer();
	//
	void FontSet();
	//�`�揈��
	void Render(RenderContext& rc);

	//�����o�ϐ�
	SpriteRender m_spriteRender;	//�X�v���C�g�����_�[
	FontRender m_fontRender;		//�t�H���g�����_�[�B
	Game* m_game;					//�Q�[��
	GameClear* m_gameclear;			//�Q�[���N���A

	bool m_isSaveClearTime = false;

	float m_minit = 0.0f;	//�b��
	int m_timer = 0.0f;		//����
};

