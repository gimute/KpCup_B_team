#include "stdafx.h"
#include "MapUi.h"

MapUi::MapUi()
{

}

MapUi::~MapUi()
{

}

namespace {
	const Vector3 MAP_POSITION = { 630.0f,-270.0f,0.0f };
	float MAP_RADIUS = 240.0f;
	float LIMITED_RANGE_IMAGE = 800.0f;
	float MAP_LIMITED_IMAGE = 300.0f;
}

bool MapUi::Start()
{
	//�}�b�v�t���[���摜��ǂݍ���
	m_mapUiFrame.Init("Assets/modelData/ui_map/MapFrame.DDS", 300.0f, 300.0f);
	//�ʒu��ݒ�
	m_mapUiFrame.SetPosition(MAP_POSITION);

	//�}�b�v�x�[�X�摜��ǂݍ���
	m_mapUiBase.Init("Assets/modelData/ui_map/MapBase.DDS", 300.0f, 300.0f);
	//�ʒu��ݒ�
	m_mapUiBase.SetPosition(MAP_POSITION);

	//�}�b�v�v���C���[�|�C���^�摜��ǂݍ���
	m_mapUiPlayerPoint.Init("Assets/modelData/ui_map/MapPlayerPoint.DDS", 30.0f, 30.0f);
	//�ʒu��ݒ�
	m_mapUiPlayerPoint.SetPosition(MAP_POSITION);

	m_game = FindGO<Game>("game");

	m_player = FindGO<Player>("player");

	CreateMapEnemyPoint(m_Initialization_Mode);

	return true;
}

void MapUi::Update()
{
	//�v���C���[�|�C���^�̍X�V(��ɉ�])
	MapPlayerPointUpdate();

	CreateMapEnemyPoint(m_Add_Mode);

	MapEnemyPointUpdate();

	EnemyPointDisplay();

	m_mapUiBase.Update();
	m_mapUiFrame.Update();	
	for (const auto& ptr : m_mapUiEnemyPointList)
	{
		ptr.second->m_mapUiEnemyPoint.Update();
	}
	m_mapUiPlayerPoint.Update();
}

void MapUi::MapPlayerPointUpdate()
{
	Vector3 CamForward = g_camera3D->GetForward();
	CamForward.Normalize();
	CamForward.y = 0.0f;

	Vector3 camup2d;

	camup2d.x = CamForward.x;
	camup2d.y = CamForward.z;

	Vector3 playerforward = m_player->m_forward;

	playerforward.y = 0.0f;

	Vector3 uiplayerforward;
	uiplayerforward.x = playerforward.x;
	uiplayerforward.y = playerforward.z;

	Quaternion rot;
	rot.SetRotation(camup2d,uiplayerforward);

	m_mapUiPlayerPoint.SetRotation(rot);
}

void MapUi::MapEnemyPointUpdate()
{
	for (const auto& ptr : m_mapUiEnemyPointList)
	{
		Vector3 CamForward = g_camera3D->GetForward();
		CamForward.Normalize();
		CamForward.y = 0.0f;

		Vector3 camup2d;

		camup2d.x = CamForward.x;
		camup2d.y = CamForward.z;

		Vector3 playerforward = ptr.second->m_enemy->m_forward;

		playerforward.y = 0.0f;

		Vector3 uiplayerforward;
		uiplayerforward.x = playerforward.x;
		uiplayerforward.y = playerforward.z;

		Quaternion rot;
		rot.SetRotation(camup2d, uiplayerforward);

		ptr.second->m_mapUiEnemyPoint.SetRotation(rot);
	}
}

void MapUi::CreateMapEnemyPoint(const CreateMapEnemyPoint_Mode mode)
{
	if (mode == CreateMapEnemyPoint_Mode::m_Initialization_Mode)
	{
		//���݂̃G�l�~�[�̐����擾
		nowenemynumber = m_game->GetEnemyListSize();

		for (const auto& ptr : m_mapUiEnemyPointList)
		{
			delete ptr.second;
		}

		m_mapUiEnemyPointList.clear();

		//���݂̃G�l�~�[�̐���for������
		for (int i = 0; i < nowenemynumber; i++)
		{
			//�G�l�~�[�̃|�C���^Ui�̍\���̂�錾(�R���X�g���N�^�ŉ摜�̃t�@�C���p�X���o�^����邽�ߕK�v�Ȃ�)
			MapUi_EnemyPoint* newEnemyPoint = new MapUi_EnemyPoint;
			//�쐬���ꂽ�\���̂̕ϐ��ɃG�l�~�[�̃C���X�^���X��o�^
			newEnemyPoint->SetEnemyInstance(*m_game->GetEnemyListInstance(i));
			//�o�^���I������ϐ���z��ɋL�^
			m_mapUiEnemyPointList.insert({ i,newEnemyPoint });
			////�����ō�������I�z��̓X�}�[�g�|�C���^�ɋL�^���Ă���
			//std::shared_ptr<MapUi_EnemyPoint> smart_ptr(newEnemyPoint);
		}

		return;
	}

	//�O�̃G�l�~�[�̐����i�[
	int oldEnemyNumber = nowenemynumber;

	//���݂̃G�l�~�[�̐����擾
	nowenemynumber = m_game->GetEnemyListSize();

	if (oldEnemyNumber > nowenemynumber)
	{
		int deleteNumber = -1;
		int forNumber = 0;
		for (const auto& ptr : m_mapUiEnemyPointList)
		{
			bool deadFlag = ptr.second->m_enemy->IsDead();

			if (deadFlag)
			{
				delete ptr.second;

				deleteNumber = forNumber;
			}
			forNumber++;
		}

		if (deleteNumber != -1)
		{
			m_mapUiEnemyPointList.erase(deleteNumber);
		}
	}

	if (oldEnemyNumber < nowenemynumber)
	{
	}

}

void MapUi::EnemyPointDisplay()
{
	Vector3 playerPos = m_player->GetPosition();
	Vector3 mapPos;

	//���݂̃G�l�~�[�̐���for������
	for (int i = 0; i < nowenemynumber; i++)
	{
		//�z��ɓo�^���ꂽ�\���̂���G�l�~�[�̈ʒu���擾
		Vector3 enemyPos = m_mapUiEnemyPointList[i]->m_enemy->m_position;

		WorldPositionConvertToMapPosition(playerPos, enemyPos, mapPos);
		//�X�v���C�g�Ɉʒu���Z�b�g
		m_mapUiEnemyPointList[i]->m_mapUiEnemyPoint.SetPosition(mapPos);
		//���E��������s��
		m_mapUiEnemyPointList[i]->m_isDraw = IsMapLimited(m_mapUiEnemyPointList[i]
			->m_mapUiEnemyPoint.GetPosition());
	}
}

void MapUi::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition
	, Vector3& mapPosition)
{
	//Y���W�̓}�b�v�̍��W�Ƃ͊֌W�Ȃ��̂ŁA0�ɂ���B
	worldCenterPosition.y = 0.0f;
	worldPosition.y = 0.0f;
	Vector3 diff = worldPosition - worldCenterPosition;

	//�x�N�g���̒������擾�B
	float length = diff.Length();

	//�J�����̑O�����x�N�g������B
	//�N�H�[�^�j�I���𐶐��B
	Vector3 forward = g_camera3D->GetForward();
	Quaternion rot;
	rot.SetRotationY(atan2(-forward.x, forward.z));

	//�x�N�g���ɃJ�����̉�]��K�p�B
	rot.Apply(diff);

	//�x�N�g���𐳋K���B
	diff.Normalize();

	//�}�b�v�̑傫��/���������ŁB
	//�x�N�g�����}�b�v���W�n�ɕϊ�����B
	diff *= length * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	//�}�b�v�̒������W�Ə�L�x�N�g�������Z����B
	mapPosition = Vector3(MAP_POSITION.x + diff.x, MAP_POSITION.y + diff.z, 0.0f);
}

bool MapUi::IsMapLimited(const Vector3 determineTargetPos)
{
	//�S�̂̔䗦����Q���
	float divisionLim = MAP_LIMITED_IMAGE / 2;
	//�}�b�v�̒��S�_���擾
	Vector3 mapUiPos = MAP_POSITION;
	//�}�b�v�̒��S�_�ɑS�̂̔䗦����Q�����������̂𑫂�
	//���ꂪ���E���ƂȂ�
	Vector3 borderLimPlus;
	borderLimPlus.x = mapUiPos.x + divisionLim;
	borderLimPlus.y = mapUiPos.y + divisionLim;
	//������������̂̓v���X�̕��Ȃ̂Ń}�C�i�X�̕������
	Vector3 bordarLimMinus;
	bordarLimMinus.x = mapUiPos.x - divisionLim;
	bordarLimMinus.y = mapUiPos.y - divisionLim;
	//���E������o����false
	if (determineTargetPos.x >= borderLimPlus.x || determineTargetPos.x <= bordarLimMinus.x
		|| determineTargetPos.y >= borderLimPlus.y || determineTargetPos.y <= bordarLimMinus.y)
	{
		return false;
	}
	//���E�����Ȃ�true
	return true;
}

void MapUi::Render(RenderContext& rc)
{
	m_mapUiBase.Draw(rc);
	m_mapUiFrame.Draw(rc);
	for (const auto& ptr : m_mapUiEnemyPointList)
	{
		if (ptr.second->m_isDraw)
		{
			ptr.second->m_mapUiEnemyPoint.Draw(rc);
		}
	}
	m_mapUiPlayerPoint.Draw(rc);
}