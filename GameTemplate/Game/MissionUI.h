#pragma once

class Game;

class MissionUI : public IGameObject
{
public:
	//�~�b�V�����X�e�[�g
	enum EnMissionState
	{
		enEnemyOllKill,	//�G��r��
		enGoDoor,		//�h�A�֌�����
		enNone			//����
	};

	MissionUI();
	~MissionUI();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//UI�̃|�W�V������ݒ�
	void SetPosition(Vector3 ps)
	{
		m_position = ps;
		m_updateFlag = true;
	}

private:
	SpriteRender m_missionSprite;		//�~�b�V�����摜
	FontRender m_mission;				//�~�b�V����

	Game* m_game = nullptr;				//

	EnMissionState m_missionState = enNone;

	int m_nowEnemyCount = 0;			//���݂̓G�̐�
	int m_maxEnemyCount = 0;			//���X�����G�̐�

	Vector3 m_position = Vector3::Zero;	//UI�̃|�W�V����

	bool m_updateFlag = false;			//���X�̃A�b�v�f�[�g�������s�����̃t���O
};

