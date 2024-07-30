#include "stdafx.h"
#include "MissionUI.h"
#include "Game.h"

namespace {
	//�~�b�V�����̃t���[���p�萔
	Vector2 MISSION_FRAME_SIZE = { 480.0f,44.0f };		//�~�b�V�����t���[���̃T�C�Y
	Vector3 MISSION_FRAME_SCALE = { 0.8f,1.0f,1.0f };	//�~�b�V�����t���[���̃X�P�[��
	Vector2 MISSION_FRAME_PIVOT = { 0.0f,0.0f };		//�~�b�V�����t���[���̃s�{�b�g
														//�Q�[���^�C�}�[�̉E���ɂ�������̂Ńs�{�b�g�͍����ɐݒ�
	Vector4 MISSION_MUL_COLOR = { 1.5f,1.5f,1.5f,1.0f };//�~�b�V�����t���[���̏�Z�J���[

	//�~�b�V�������p�萔
	Vector3 MISSION_STRING_POSITION = { 0.0f,50.0f,0.0f };		//���̈ʒu���~�b�V�����t���[���ɍ��킹�邽�߂�Vector
	float MISSION_STRING_SCALE = 0.4f;							//���̃X�P�[��
	Vector4 MISSION_STRING_COLOR = { 0.68f,0.68f,0.68f,1.0f };	//�����̐F
	float MISSION_STRING_SHADOW_OFFSET = 0.5;					//�����̉e�̃I�t�Z�b�g�A�e�͕����𑾂����邽�߂Ɏg���Ă���
}

MissionUI::MissionUI()
{
	
}

MissionUI::~MissionUI()
{

}

bool MissionUI::Start()
{
	//�ŏ��̃~�b�V�����X�e�[�g��ݒ�
	m_missionState = enEnemyOllKill;

	//�~�b�V�����t���[���̏��X�ݒ�
	m_missionSprite.Init("Assets/sprite/MissionUI/missionUIFrame.DDS", MISSION_FRAME_SIZE.x, MISSION_FRAME_SIZE.y);
	m_missionSprite.SetScale(MISSION_FRAME_SCALE);
	m_missionSprite.SetPivot(MISSION_FRAME_PIVOT);
	m_missionSprite.SetMulColor(MISSION_MUL_COLOR);
	m_missionSprite.Update();

	//�~�b�V���������X�ݒ�
	m_missionString.SetPosition(MISSION_STRING_POSITION);
	m_missionString.SetScale(MISSION_STRING_SCALE);
	m_missionString.SetColor(MISSION_STRING_COLOR);
	//�������ׂ������̂ŕ����Ɠ����F�̉e��t���Ė�����葾������
	m_missionString.SetShadowParam(true, MISSION_STRING_SHADOW_OFFSET, MISSION_STRING_COLOR);

	//�Q�[���̃C���X�^���X�̃|�C���^���󂯎��
	m_game = FindGO<Game>("game");

	//���̃N���X�̃I�u�W�F�N�g����������̓G�̐����A�|���G�̐��Ƃ��Đݒ肷��
	m_defeatEnemyNumber = m_game->GetEnemyListSize();
	//�ꉞ���݂̓G�̐���������Ă���
	m_nowEnemyCount = m_defeatEnemyNumber;

	return true;
}

void MissionUI::Update()
{
	//�A�b�v�f�[�g�t���O��turu�Ȃ�
	if (m_updateFlag)
	{
		//�X�v���C�g�̃|�W�V������ݒ�
		m_missionSprite.SetPosition(m_position);
		m_missionSprite.Update();

		//�~�b�V�������̃|�W�V������ݒ�
		m_missionString.SetPosition(MISSION_STRING_POSITION + (m_position * 1.2f));
	}

	switch (m_missionState)
	{
	case MissionUI::enEnemyOllKill:
		//���݂̓G�̐����擾
		m_nowEnemyCount = m_game->GetEnemyListSize();

		//�G�����Ȃ��Ȃ�����
		if (m_nowEnemyCount <= 0)
		{
			//�~�b�V�����X�e�[�g���h�A�Ɍ������ɕύX
			m_missionState = enGoDoor;
		}

		//missionString�ɓ���镶�̏���
		wchar_t wcsbuf[256];
		swprintf_s(wcsbuf, 256, L" �G��S�ē|��  �c��%02d/%02d", int(m_nowEnemyCount), int(m_defeatEnemyNumber));

		//�~�b�V��������ݒ�
		m_missionString.SetText(wcsbuf);
		break;

	case MissionUI::enGoDoor:
		//�~�b�V��������ݒ�
		m_missionString.SetText(L" �h�A�Ɍ�����");
		break;

	case MissionUI::enNone:
		break;

	default:
		break;
	}
}

void MissionUI::Render(RenderContext& rc)
{
	if (m_DrawFlag)
	{
		m_missionSprite.Draw(rc);
		m_missionString.Draw(rc);
	}
}
