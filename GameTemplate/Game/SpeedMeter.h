#pragma once
class Player;
class SpeedMeter : public IGameObject
{
public:
	SpeedMeter();
	~SpeedMeter();

	bool Start();
	//�X�V�����B
	void Update();

	void Render(RenderContext& rc);

//�����o�ϐ�
	FontRender m_fontrender;	//�t�H���g�����_�[
	Player* m_player;
};

