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

	void SetDrawFlag(bool flag)
	{
		m_DrawFlag = flag;
	}

private:
	SpriteRender m_missionSprite;		//�~�b�V�����t���[��
	FontRender m_missionString;			//�~�b�V������
	Vector3 m_position = Vector3::Zero;	//UI�̃|�W�V����
	int m_nowEnemyCount = 0;			//���݂̓G�̐�
	int m_defeatEnemyNumber = 0;		//�|���G�̐�
	Game* m_game = nullptr;				//�Q�[��
	EnMissionState m_missionState = enNone;	//�~�b�V�����X�e�[�g
	bool m_updateFlag = false;			//�A�b�v�f�[�g�������s�����̃t���O
	bool m_DrawFlag = true;				//�`�悷�邩�ǂ����̃t���O
};

