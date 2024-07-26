#include "stdafx.h"
#include "MissionUI.h"
#include "Game.h"

namespace {
	MissionUI::EnMissionState START_MISSION = MissionUI::EnMissionState::enEnemyOllKill;	//�ŏ��̃~�b�V�����͓G�����ׂē|��

	Vector2 MISSION_FRAME_SIZE = { 480.0f,44.0f };		//�~�b�V�����t���[���̃T�C�Y
	Vector3 MISSION_FRAME_SCALE = { 1.0f,1.0f,1.0f };	//�~�b�V�����t���[���̃X�P�[��
	Vector2 MISSION_FRAME_PIVOT = { 0.0f,0.0f };		//�~�b�V�����t���[���̃s�{�b�g
														//�Q�[���^�C�}�[�̉E���ɂ�������̂Ńs�{�b�g�͍����ɐݒ�

	Vector3 MISSION_POSITION = { 0.0f,50.0f,0.0f };	//�����̈ʒu���~�b�V�����t���[���ɍ��킹�邽�߂�Vector
	float MISSION_SCALE = 0.4f;
	Vector4 MISSION_COLOR = { 0.68f,0.68f,0.68f,1.0f };
}

MissionUI::MissionUI()
{
	
}

MissionUI::~MissionUI()
{

}

bool MissionUI::Start()
{
	m_missionState = START_MISSION;

	m_missionSprite.Init("Assets/sprite/MissionUI/missionUIFrame.DDS", MISSION_FRAME_SIZE.x, MISSION_FRAME_SIZE.y);
	m_missionSprite.SetScale(MISSION_FRAME_SCALE);
	m_missionSprite.SetPivot(MISSION_FRAME_PIVOT);

	m_missionSprite.Update();

	m_mission.SetPosition(MISSION_POSITION);
	m_mission.SetScale(MISSION_SCALE);
	m_mission.SetColor(MISSION_COLOR);

	m_mission.SetShadowParam(true, 0.5f, MISSION_COLOR);

	m_game = FindGO<Game>("game");

	m_maxEnemyCount = m_game->GetEnemyListSize();
	m_nowEnemyCount = m_maxEnemyCount;

	return true;
}

void MissionUI::Update()
{
	if (m_updateFlag)
	{
		m_missionSprite.SetPosition(m_position);
		m_missionSprite.Update();

		m_mission.SetPosition(MISSION_POSITION + (m_position * 1.2f));
	}

	switch (m_missionState)
	{
	case MissionUI::enEnemyOllKill:

		m_nowEnemyCount = m_game->GetEnemyListSize();

		wchar_t wcsbuf[256];
		swprintf_s(wcsbuf, 256, L"�G��S�ē|��  �c��%02d/%02d", int(m_nowEnemyCount), int(m_maxEnemyCount));

		m_mission.SetText(wcsbuf);
		break;

	case MissionUI::enGoDoor:
		break;

	case MissionUI::enNone:
		break;

	default:
		break;
	}
}

void MissionUI::Render(RenderContext& rc)
{
	m_missionSprite.Draw(rc);
	m_mission.Draw(rc);
}
